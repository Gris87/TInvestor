import asyncio
import argparse
import logging
import os
import sys
import time
from aiostream import stream
from decimal import Decimal
from loguru import logger

from tinkoff.invest import InstrumentIdType, GetMaxLotsRequest, GetOperationsByCursorRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings
from tinkoff.invest.schemas import OperationState, OrderIdType
from tinkoff.invest.utils import quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c"


async def follow(args):
    if args.official_dest and not args.confirm:
        answer = input("Are you sure to use official destination account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return

    logger.info("Connecting to server")

    src_token = _get_token(args.src_token, args.src_token_file)
    dest_token = _get_token(args.dest_token, args.dest_token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10)

    async with AsyncRetryingClient(src_token, settings=retry_settings, target=INVEST_GRPC_API if args.official_src else INVEST_GRPC_API_SANDBOX) as src_client:
        async with AsyncRetryingClient(dest_token, settings=retry_settings, target=INVEST_GRPC_API if args.official_dest else INVEST_GRPC_API_SANDBOX) as dest_client:
            logger.info("Verifying accounts")

            if not await _validate_account(src_client, args.src_account, "Source"):
                sys.exit(1)

                return

            if not await _validate_account(dest_client, args.dest_account, "Destination"):
                sys.exit(1)

                return

            src_portfolio = await _get_valid_portfolio(src_client, args.src_account)
            dest_portfolio = await _get_valid_portfolio(dest_client, args.dest_account)

            await _handle_portfolios(dest_client, args.dest_account, src_portfolio, dest_portfolio)

            if args.mode == "stream":
                await _start_follow_stream_mode(src_client, dest_client, args.src_account, args.dest_account)
            elif args.mode == "poll":
                await _start_follow_poll_mode(src_client, dest_client, args.src_account, args.dest_account)
            elif args.mode == "poll-file":
                await _start_follow_poll_file_mode(src_client, dest_client, args.src_account, args.dest_account, args.polling_file)


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


async def _validate_account(client, account_id, direction):
    accounts = {}

    tinkoff_accounts = await client.users.get_accounts()

    for account in tinkoff_accounts.accounts:
        accounts[account.id] = account.name

    if account_id not in accounts:
        logger.error(f'{direction} account "{account_id}" not found. Please use one of the following')

        for id, name in accounts.items():
            print(f"{id:36} | {name}")

        return False

    return True


async def _start_follow_stream_mode(src_client, dest_client, src_account, dest_account):
    src_stream = src_client.operations_stream.portfolio_stream(accounts=[src_account])
    dest_stream = dest_client.operations_stream.portfolio_stream(accounts=[dest_account])

    async with stream.merge(src_stream, dest_stream).stream() as streamer:
        async for x in streamer:
            if x.portfolio is not None:
                src_portfolio = await _get_valid_portfolio(src_client, src_account)
                dest_portfolio = await _get_valid_portfolio(dest_client, dest_account)

                await _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio)


async def _start_follow_poll_mode(src_client, dest_client, src_account, dest_account):
    src_timestamp = await _get_last_operation_timestamp(src_client, src_account)
    dest_timestamp = await _get_last_operation_timestamp(dest_client, dest_account)

    last_modified = src_timestamp + dest_timestamp

    while True:
        src_timestamp = await _get_last_operation_timestamp(src_client, src_account)
        dest_timestamp = await _get_last_operation_timestamp(dest_client, dest_account)

        current_modified = src_timestamp + dest_timestamp

        if current_modified != last_modified:
            last_modified = current_modified

            src_portfolio = await _get_valid_portfolio(src_client, src_account)
            dest_portfolio = await _get_valid_portfolio(dest_client, dest_account)

            await _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio)

        await asyncio.sleep(10)


async def _start_follow_poll_file_mode(src_client, dest_client, src_account, dest_account, polling_file):
    last_modified = os.path.getmtime(polling_file)

    while True:
        current_modified = os.path.getmtime(polling_file)

        if current_modified != last_modified:
            last_modified = current_modified

            src_portfolio = await _get_valid_portfolio(src_client, src_account)
            dest_portfolio = await _get_valid_portfolio(dest_client, dest_account)

            await _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio)

        await asyncio.sleep(1)


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


async def _get_last_operation_timestamp(client, account):
    req = GetOperationsByCursorRequest(
        account_id=account,
        limit=1,
        state=OperationState.OPERATION_STATE_EXECUTED,
        cursor="",
        without_trades=True
    )

    operations = await client.operations.get_operations_by_cursor(req)

    if len(operations.items) > 0:
        return int(operations.items[0].date.timestamp() * 1000)

    return 0


async def _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio):
    src_instrument_to_cost, src_total_cost = _build_instrument_to_cost_map(src_portfolio)
    dest_instrument_to_cost, dest_total_cost = _build_instrument_to_cost_map(dest_portfolio)

    instruments_for_sale, instruments_for_buy = await _build_instruments_for_trading(dest_client, src_instrument_to_cost, dest_instrument_to_cost, src_total_cost, dest_total_cost)

    if instruments_for_sale or instruments_for_buy:
        logger.info("Synchronization in progress")

        if instruments_for_sale:
            await _trade_instruments(dest_client, dest_account, instruments_for_sale)

        if instruments_for_buy:
            await _trade_instruments(dest_client, dest_account, instruments_for_buy)

        logger.info("Synchronized")


def _build_instrument_to_cost_map(portfolio):
    res = {}
    total_cost = Decimal("0.00")

    for position in portfolio.positions:
        if position.instrument_uid == RUBLE_UID:
            item = {
                "price": 1.0,
                "cost": quotation_to_decimal(position.quantity)
            }
        else:
            item = {
                "price": quotation_to_decimal(position.current_price),
                "cost": quotation_to_decimal(position.quantity) * quotation_to_decimal(position.average_position_price)
            }

        res[position.instrument_uid] = item
        total_cost += item["cost"]

    del res[RUBLE_UID]

    return res, total_cost


async def _build_instruments_for_trading(dest_client, src_instrument_to_cost, dest_instrument_to_cost, src_total_cost, dest_total_cost):
    instruments_for_sale = {}
    instruments_for_buy = {}

    for src_instrument_id, src_portfolio_item in src_instrument_to_cost.items():
        src_part = src_portfolio_item["cost"] / src_total_cost
        expected_cost = src_part * dest_total_cost

        if src_instrument_id not in dest_instrument_to_cost:
            instruments_for_buy[src_instrument_id] = expected_cost

            continue

        lot = await _get_instrument_lot(dest_client, src_instrument_id)

        dest_portfolio_item = dest_instrument_to_cost[src_instrument_id]
        delta = expected_cost - dest_portfolio_item["cost"]
        lot_price = dest_portfolio_item["price"] * lot

        if delta < -lot_price:
            instruments_for_sale[src_instrument_id] = expected_cost
        elif delta > lot_price:
            instruments_for_buy[src_instrument_id] = expected_cost

    for dest_instrument_id in dest_instrument_to_cost:
        if dest_instrument_id not in src_instrument_to_cost:
            instruments_for_sale[dest_instrument_id] = Decimal(0)  # Need to sell all

    return instruments_for_sale, instruments_for_buy


async def _trade_instruments(dest_client, dest_account, instruments):
    tasks = []

    dest_portfolio = await _get_valid_portfolio(dest_client, dest_account)

    for instrument_id, expected_cost in instruments.items():
        current_cost = Decimal(0)

        for position in dest_portfolio.positions:
            if position.instrument_uid == instrument_id:
                current_cost = quotation_to_decimal(position.quantity) * quotation_to_decimal(position.average_position_price)

                break

        delta = expected_cost - current_cost

        if delta > 0:
            tasks.append(_buy(dest_client, dest_account, instrument_id, delta))
        else:
            tasks.append(_sell(dest_client, dest_account, instrument_id, -delta, expected_cost == Decimal(0)))

    await asyncio.gather(*tasks)


async def _buy(dest_client, dest_account, instrument_id, delta):
    lot = await _get_instrument_lot(dest_client, instrument_id)

    order_id = None
    order_price = None

    while True:
        if order_id is not None:
            resp = await dest_client.orders.get_order_state(
                account_id=dest_account,
                order_id=order_id,
                price_type=PriceType.PRICE_TYPE_CURRENCY,
                order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
            )

            if resp.execution_report_status == OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_FILL:
                break

        order_book = await dest_client.market_data.get_order_book(depth=1, instrument_id=instrument_id)
        price = _calculate_buy_price(order_book)

        if price is not None:
            if price != order_price:
                if order_id is not None:
                    resp = await dest_client.orders.get_order_state(
                        account_id=dest_account,
                        order_id=order_id,
                        price_type=PriceType.PRICE_TYPE_CURRENCY,
                        order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
                    )

                    delta -= resp.lots_executed * lot * quotation_to_decimal(resp.executed_order_price)

                    await dest_client.orders.cancel_order(
                        account_id=dest_account,
                        order_id=order_id,
                        order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
                    )

                    order_id = None
                    order_price = None

                price_decimal = quotation_to_decimal(price)
                lot_price = lot * price_decimal

                req = GetMaxLotsRequest(account_id=dest_account, instrument_id=instrument_id, price=price)
                max_lots = await dest_client.orders.get_max_lots(req)

                delta_quantity = round(delta / lot_price)
                amount_to_buy = min(delta_quantity, max_lots.buy_limits.buy_max_lots)

                if amount_to_buy > 0:
                    resp = await dest_client.orders.post_order(
                        quantity=amount_to_buy,
                        price=price,
                        direction=OrderDirection.ORDER_DIRECTION_BUY,
                        account_id=dest_account,
                        order_type=OrderType.ORDER_TYPE_LIMIT,
                        instrument_id=instrument_id,
                        time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                        price_type=PriceType.PRICE_TYPE_CURRENCY
                    )

                    if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                        order_id = resp.order_id
                        order_price = price
                else:
                    break

        await asyncio.sleep(30)


async def _sell(dest_client, dest_account, instrument_id, delta, sell_all):
    lot = await _get_instrument_lot(dest_client, instrument_id)

    order_id = None
    order_price = None

    while True:
        if order_id is not None:
            resp = await dest_client.orders.get_order_state(
                account_id=dest_account,
                order_id=order_id,
                price_type=PriceType.PRICE_TYPE_CURRENCY,
                order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
            )

            if resp.execution_report_status == OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_FILL:
                break

        order_book = await dest_client.market_data.get_order_book(depth=1, instrument_id=instrument_id)
        price = _calculate_sell_price(order_book)

        if price is not None:
            if price != order_price:
                if order_id is not None:
                    resp = await dest_client.orders.get_order_state(
                        account_id=dest_account,
                        order_id=order_id,
                        price_type=PriceType.PRICE_TYPE_CURRENCY,
                        order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
                    )

                    delta -= resp.lots_executed * lot * quotation_to_decimal(resp.executed_order_price)

                    await dest_client.orders.cancel_order(
                        account_id=dest_account,
                        order_id=order_id,
                        order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
                    )

                    order_id = None
                    order_price = None

                price_decimal = quotation_to_decimal(price)
                lot_price = lot * price_decimal

                req = GetMaxLotsRequest(account_id=dest_account, instrument_id=instrument_id, price=price)
                max_lots = await dest_client.orders.get_max_lots(req)

                if sell_all:
                    amount_to_sell = max_lots.sell_limits.sell_max_lots
                else:
                    delta_quantity = round(delta / lot_price)
                    amount_to_sell = min(delta_quantity, max_lots.sell_limits.sell_max_lots)

                if amount_to_sell > 0:
                    resp = await dest_client.orders.post_order(
                        quantity=amount_to_sell,
                        price=price,
                        direction=OrderDirection.ORDER_DIRECTION_SELL,
                        account_id=dest_account,
                        order_type=OrderType.ORDER_TYPE_LIMIT,
                        instrument_id=instrument_id,
                        time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                        price_type=PriceType.PRICE_TYPE_CURRENCY
                    )

                    if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                        order_id = resp.order_id
                        order_price = price
                else:
                    break

        await asyncio.sleep(30)


# TODO: Clear at the start of the day
_instrument_lot_cache = {}


async def _get_instrument_lot(client, instrument_id):
    global _instrument_lot_cache

    if instrument_id in _instrument_lot_cache:
        return _instrument_lot_cache[instrument_id]

    resp = await client.instruments.get_instrument_by(id_type=InstrumentIdType.INSTRUMENT_ID_TYPE_UID, id=instrument_id)
    _instrument_lot_cache[instrument_id] = resp.instrument.lot

    return resp.instrument.lot


def _calculate_buy_price(order_book):
    if order_book.asks:
        return order_book.asks[0].price

    if order_book.bids:
        return order_book.bids[0].price

    return None


def _calculate_sell_price(order_book):
    if order_book.bids:
        return order_book.bids[0].price

    if order_book.asks:
        return order_book.asks[0].price

    return None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--official-src",
        dest="official_src",
        default=False,
        action="store_true",
        help="Flag for using official server for source account",
    )
    parser.add_argument(
        "--official-dest",
        dest="official_dest",
        default=False,
        action="store_true",
        help="Flag for using official server for destination account",
    )
    parser.add_argument(
        "--confirm",
        dest="confirm",
        default=False,
        action="store_true",
        help="Do not ask about using official destination account",
    )
    parser.add_argument(
        "--src-token",
        dest="src_token",
        type=str,
        default="",
        help="Token for Tinkoff API (Source account)",
    )
    parser.add_argument(
        "--dest-token",
        dest="dest_token",
        type=str,
        default="",
        help="Token for Tinkoff API (Destination account)",
    )
    parser.add_argument(
        "--src-token-file",
        dest="src_token_file",
        type=str,
        default="",
        help="Path to file with token for Tinkoff API (Source account)",
    )
    parser.add_argument(
        "--dest-token-file",
        dest="dest_token_file",
        type=str,
        default="",
        help="Path to file with token for Tinkoff API (Destination account)",
    )
    parser.add_argument(
        "--src-account",
        dest="src_account",
        type=str,
        default="",
        help="Source account ID",
    )
    parser.add_argument(
        "--dest-account",
        dest="dest_account",
        type=str,
        default="",
        help="Destination account ID",
    )
    parser.add_argument(
        "--mode",
        dest="mode",
        type=str,
        choices=["stream", "poll", "poll-file"],
        default="stream",
        help="Mode for detecting portfolio changes",
    )
    parser.add_argument(
        "--polling-file",
        dest="polling_file",
        type=str,
        default="",
        help="Path to file for polling (only for --mode poll-file)",
    )
    args = parser.parse_args()

    if (args.src_token == "" and args.src_token_file == "") or (args.src_token != "" and args.src_token_file != ""):
        logger.error("Please specify source token with --src-token or --src-token-file")

        sys.exit(1)

    if (args.dest_token == "" and args.dest_token_file == "") or (args.dest_token != "" and args.dest_token_file != ""):
        logger.error("Please specify destination token with --dest-token or --dest-token-file")

        sys.exit(1)

    if args.src_account == "":
        logger.error("Please specify source account ID with --src-account")

        sys.exit(1)

    if args.dest_account == "":
        logger.error("Please specify destination account ID with --dest-account")

        sys.exit(1)

    if args.mode == "poll-file" and args.polling_file == "":
        logger.error("Please specify path to file for polling with --polling-file")

        sys.exit(1)

    if args.mode != "poll-file" and args.polling_file != "":
        logger.error("Path to file for polling specified without --mode poll-file")

        sys.exit(1)

    asyncio.run(follow(args))
