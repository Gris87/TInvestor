import asyncio
import argparse
import logging
import sys
import time

from datetime import datetime, timedelta
from decimal import Decimal
from loguru import logger

from t_tech.invest import GetMaxLotsRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from t_tech.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings
from t_tech.invest.schemas import OrderIdType
from t_tech.invest.utils import decimal_to_quotation, quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c"


HUNDRED_PERCENT = 100.0

buy_order_id = None


async def fall_trading(args):
    if args.official and not args.confirm:
        answer = input("Are you sure to use official account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return

    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10000)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API if args.official else INVEST_GRPC_API_SANDBOX) as client:
        logger.info("Verifying account")

        if not await _validate_account(client, args.account):
            sys.exit(1)

            return

        await _start_orderbook_streaming(client, args.account, args.instrument_id, args.fall, args.limit_lots, args.limit_by_time)
        await _cancel_order(client, args.account)


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


async def _start_orderbook_streaming(client, account, instrument_id, fall, limit_lots, limit_by_time):
    start_time = datetime.now()

    while True:
        if limit_by_time > 0 and datetime.now() - start_time > timedelta(minutes=limit_by_time):
            break

        orderbook = await client.market_data.get_order_book(instrument_id=instrument_id, depth=50)
        await _handle_orderbook(client, account, instrument_id, fall, limit_lots, orderbook)

        await asyncio.sleep(1)


async def _handle_orderbook(client, account, instrument_id, fall, limit_lots, orderbook):
    portfolio = await _get_valid_portfolio(client, account)
    amount_of_lots = 0

    for position in portfolio.positions:
        if position.instrument_uid == instrument_id:
            amount_of_lots = int(quotation_to_decimal(position.quantity_lots))

            break

    if amount_of_lots < limit_lots and len(orderbook.bids) > 0 and len(orderbook.asks) > 0:
        top_ask_price = quotation_to_decimal(orderbook.asks[0].price)
        last_price = quotation_to_decimal(orderbook.last_price)
        base_price = min(top_ask_price, last_price);
        maximum_buy_price = base_price * Decimal(1 - (fall / HUNDRED_PERCENT))

        price = decimal_to_quotation(maximum_buy_price)

        for bid in orderbook.bids:
            if quotation_to_decimal(bid.price) <= maximum_buy_price:
                price = bid.price

                break

        await _buy(client, account, instrument_id, limit_lots - amount_of_lots, price)


async def _get_valid_portfolio(client, account):
    while True:
        portfolio = await client.operations.get_portfolio(account_id=account)

        good = True

        for position in portfolio.positions:
            if position.instrument_uid==RUBLE_UID:
                continue

            if position.average_position_price.units <= 0 and position.average_position_price.nano <= 0:
                good = False

                break

        if good:
            return portfolio

        await asyncio.sleep(1)


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
                logger.info(f"Buy {amount_to_buy} lots with price {quotation_to_decimal(price)}")

                buy_order_id = resp.order_id

                break
        else:
            break

        await asyncio.sleep(1)


async def _cancel_order(client, account):
    logger.info(f"Cancel order")

    global buy_order_id

    if buy_order_id is not None:
        await client.orders.cancel_order(
            account_id=account,
            order_id=buy_order_id,
            order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
        )

        buy_order_id = None


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
        "--fall",
        dest="fall",
        type=float,
        default=3.0,
        help="Set buy price on instant fall in percent",
    )
    parser.add_argument(
        "--limit-lots",
        dest="limit_lots",
        type=int,
        default=1,
        help="Amount of lots to buy",
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

    if args.fall <= 0:
        logger.error("Please specify valid fall value with --fall")

        sys.exit(1)

    if args.limit_lots <= 0:
        logger.error("Invalid --limit-lots")

        sys.exit(1)

    if args.limit_by_time < 0:
        logger.error("Invalid --limit-by-time")

        sys.exit(1)

    asyncio.run(fall_trading(args))
