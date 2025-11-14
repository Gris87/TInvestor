import asyncio
import argparse
import logging
import os
import sys
import time
from aiostream import stream
from datetime import datetime, timedelta
from decimal import Decimal
from loguru import logger

from tinkoff.invest import GetMaxLotsRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings
from tinkoff.invest.schemas import OrderBookInstrument, OrderIdType
from tinkoff.invest.utils import quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


HUNDRED_PERCENT       = 100.0
MINIMUM_YIELD_PERCENT = 0.30
COMMISSION            = 0.04

buy_order_id = None
sell_order_id = None


async def asap_trading(args):
    if args.official and not args.confirm:
        answer = input("Are you sure to use official account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return

    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API if args.official else INVEST_GRPC_API_SANDBOX) as client:
        logger.info("Verifying account")

        if not await _validate_account(client, args.account):
            return

        await _start_orderbook_streaming(client, args.account, args.instrument_id, args.spread, args.limit_lots, args.limit_by_time)
        await _cancel_orders(client, args.account)

    return


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


async def _validate_account(client, account_id):
    accounts = {}

    tinkoff_accounts = await client.users.get_accounts()

    for account in tinkoff_accounts.accounts:
        accounts[account.id] = account.name

    if account_id not in accounts:
        logger.error(f'Account "{account_id}" not found. Please use one of the following')

        for id, name in accounts.items():
            print(f"{id:36} | {name}")

        return False

    return True


async def _start_orderbook_streaming(client, account, instrument_id, spread, limit_lots, limit_by_time):
    start_time = datetime.now()

    while True:
        if limit_by_time > 0 and datetime.now() - start_time > timedelta(minutes=limit_by_time):
            break

        orderbook = await client.market_data.get_order_book(instrument_id=instrument_id, depth=50)
        await _handle_orderbook(client, account, instrument_id, spread, limit_lots, orderbook)

        await asyncio.sleep(1)


async def _handle_orderbook(client, account, instrument_id, spread, limit_lots, orderbook):
    tasks = []

    portfolio = await client.operations.get_portfolio(account_id=account)
    amount_of_lots = 0
    avg_price = Decimal(0)

    for position in portfolio.positions:
        if position.instrument_uid == instrument_id:
            amount_of_lots = int(quotation_to_decimal(position.quantity))
            avg_price = quotation_to_decimal(position.average_position_price)

            break

    if amount_of_lots < limit_lots and len(orderbook.bids) > 0:
        maximum_buy_price = quotation_to_decimal(orderbook.bids[0].price)

        if amount_of_lots > 0:
            maximum_buy_price = min(maximum_buy_price, avg_price * Decimal(1 - (spread / HUNDRED_PERCENT)))

        if len(orderbook.asks) > 0:
            maximum_buy_price = min(maximum_buy_price, quotation_to_decimal(orderbook.asks[0].price) * Decimal(1 - (spread / HUNDRED_PERCENT)))

        for bid in orderbook.bids:
            if quotation_to_decimal(bid.price) <= maximum_buy_price:
                tasks.append(_buy(client, account, instrument_id, limit_lots - amount_of_lots, orderbook.bids[0].price))

                break

    if amount_of_lots > 0 and len(orderbook.asks) > 0:
        minimum_sell_price = avg_price * Decimal(1 + (MINIMUM_YIELD_PERCENT + (2 * COMMISSION)) / HUNDRED_PERCENT)

        for ask in orderbook.asks:
            if quotation_to_decimal(ask.price) >= minimum_sell_price:
                tasks.append(_sell(client, account, instrument_id, amount_of_lots, ask.price))

                break

    if len(tasks) > 0:
        await asyncio.gather(*tasks)


async def _buy(client, account, instrument_id, amount_of_lots, price):
    global buy_order_id

    if buy_order_id is not None:
        order_state = await client.orders.get_order_state(
            account_id=account,
            order_id=buy_order_id,
            price_type=PriceType.PRICE_TYPE_CURRENCY,
            order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
        )

        if order_state.execution_report_status in [OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_NEW, OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_PARTIALLYFILL]:
            lots_remained = order_state.lots_requested - order_state.lots_executed

            if order_state.initial_security_price.units == price.units and order_state.initial_security_price.nano == price.nano and lots_remained == amount_of_lots:
                return

            await client.orders.cancel_order(
                account_id=account,
                order_id=buy_order_id,
                order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
            )

        buy_order_id = None

    while True:
        req = GetMaxLotsRequest(account_id=account, instrument_id=instrument_id, price=price)
        max_lots = await client.orders.get_max_lots(req)

        amount_to_buy = min(amount_of_lots, max_lots.buy_limits.buy_max_lots)

        if amount_to_buy > 0:
            resp = await client.orders.post_order(
                quantity=amount_to_buy,
                price=price,
                direction=OrderDirection.ORDER_DIRECTION_BUY,
                account_id=account,
                order_type=OrderType.ORDER_TYPE_LIMIT,
                instrument_id=instrument_id,
                time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                price_type=PriceType.PRICE_TYPE_CURRENCY
            )

            if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                logger.info(f"Buy {amount_of_lots} lots with price {quotation_to_decimal(price)}")

                buy_order_id = resp.order_id

                break
        else:
            break

        await asyncio.sleep(1)


async def _sell(client, account, instrument_id, amount_of_lots, price):
    global sell_order_id

    if sell_order_id is not None:
        order_state = await client.orders.get_order_state(
            account_id=account,
            order_id=sell_order_id,
            price_type=PriceType.PRICE_TYPE_CURRENCY,
            order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
        )

        if order_state.execution_report_status in [OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_NEW, OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_PARTIALLYFILL]:
            lots_remained = order_state.lots_requested - order_state.lots_executed

            if order_state.initial_security_price.units == price.units and order_state.initial_security_price.nano == price.nano and lots_remained == amount_of_lots:
                return

            await client.orders.cancel_order(
                account_id=account,
                order_id=sell_order_id,
                order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
            )

        sell_order_id = None

    while True:
        req = GetMaxLotsRequest(account_id=account, instrument_id=instrument_id, price=price)
        max_lots = await client.orders.get_max_lots(req)

        amount_to_sell = min(amount_of_lots, max_lots.sell_limits.sell_max_lots)

        if amount_to_sell > 0:
            resp = await client.orders.post_order(
                quantity=amount_to_sell,
                price=price,
                direction=OrderDirection.ORDER_DIRECTION_SELL,
                account_id=account,
                order_type=OrderType.ORDER_TYPE_LIMIT,
                instrument_id=instrument_id,
                time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                price_type=PriceType.PRICE_TYPE_CURRENCY
            )

            if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                logger.info(f"Sell {amount_of_lots} lots with price {quotation_to_decimal(price)}")

                sell_order_id = resp.order_id

                break
        else:
            break

        await asyncio.sleep(1)


async def _cancel_orders(client, account):
    logger.info(f"Cancel orders")

    global buy_order_id
    global sell_order_id

    if buy_order_id is not None:
        await client.orders.cancel_order(
            account_id=account,
            order_id=buy_order_id,
            order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
        )

        buy_order_id = None

    if sell_order_id is not None:
        await client.orders.cancel_order(
            account_id=account,
            order_id=sell_order_id,
            order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
        )

        sell_order_id = None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--official",
        dest="official",
        default=False,
        action="store_true",
        help="Flag for using official server",
    )
    parser.add_argument(
        "--confirm",
        dest="confirm",
        default=False,
        action="store_true",
        help="Do not ask about using official account",
    )
    parser.add_argument(
        "--token",
        dest="token",
        type=str,
        default="",
        help="Token for Tinkoff API",
    )
    parser.add_argument(
        "--token-file",
        dest="token_file",
        type=str,
        default="",
        help="Path to file with token for Tinkoff API",
    )
    parser.add_argument(
        "--account",
        dest="account",
        type=str,
        default="",
        help="Account ID",
    )
    parser.add_argument(
        "--instrument-id",
        dest="instrument_id",
        type=str,
        default="",
        help="Instrument ID",
    )
    parser.add_argument(
        "--spread",
        dest="spread",
        type=float,
        default=0.7,
        help="Keep spread between bid and maximum ask",
    )
    parser.add_argument(
        "--limit-lots",
        dest="limit_lots",
        type=int,
        default=1,
        help="Amount of lots to keep",
    )
    parser.add_argument(
        "--limit-by-time",
        dest="limit_by_time",
        type=int,
        default=0,
        help="Amount of minutes to work (0 - unlimit)",
    )
    args = parser.parse_args()

    if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
        logger.error("Please specify token with --token or --token-file")

        sys.exit(1)

    if args.account == "":
        logger.error("Please specify account ID with --account")

        sys.exit(1)

    if args.instrument_id == "":
        logger.error("Please specify instrument ID with --instrument-id")

        sys.exit(1)

    if args.limit_lots <= 0:
        logger.error("Invalid --limit-lots")

        sys.exit(1)

    if args.limit_by_time < 0:
        logger.error("Invalid --limit-by-time")

        sys.exit(1)

    asyncio.run(asap_trading(args))
