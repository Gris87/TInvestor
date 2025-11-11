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

from tinkoff.invest import InstrumentIdType, GetMaxLotsRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings
from tinkoff.invest.schemas import OrderBookInstrument, OrderIdType
from tinkoff.invest.utils import quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


HUNDRED_PERCENT       = 100.0
MINIMUM_YIELD_PERCENT = 0.30
COMMISSION            = 0.04


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

        await _start_orderbook_streaming(client, args.account, args.instrument_id, args.limit_lots, args.limit_by_time)
        await _cancel_orders(client)

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


async def _start_orderbook_streaming(client, account, instrument_id, limit_lots, limit_by_time):
    start_time = datetime.now()

    stream = client.create_market_data_stream()
    stream.order_book.subscribe(
        [
            OrderBookInstrument(
                instrument_id=instrument_id,
                depth=50,
            )
        ]
    )

    orderbook = await client.market_data.get_order_book(instrument_id=instrument_id, depth=50)
    await _handle_orderbook(client, account, instrument_id, limit_lots, orderbook)

    async for x in stream:
        if limit_by_time > 0 and datetime.now() - start_time > timedelta(minutes=limit_by_time):
            break

        if x.orderbook is not None:
            await _handle_orderbook(client, account, instrument_id, limit_lots, x.orderbook)


async def _handle_orderbook(client, account, instrument_id, limit_lots, orderbook):
    tasks = []

    portfolio = await client.operations.get_portfolio(account_id=account)
    amount_of_lots = Decimal(0)
    avg_price = Decimal(0)

    for position in portfolio.positions:
        if position.instrument_uid == instrument_id:
            amount_of_lots = quotation_to_decimal(position.quantity)
            avg_price = quotation_to_decimal(position.average_position_price)

            break

    if amount_of_lots < limit_lots and len(orderbook.bids) > 0:
        tasks.append(_buy(client, account, instrument_id, limit_lots - amount_of_lots, orderbook.bids[0].price))

    if amount_of_lots > 0 and len(orderbook.asks) > 0:
        minimum_sell_price = avg_price * Decimal(1 + (MINIMUM_YIELD_PERCENT + (2 * COMMISSION)) / HUNDRED_PERCENT)

        for ask in orderbook.asks:
            if quotation_to_decimal(ask.price) >= minimum_sell_price:
                tasks.append(_sell(client, account, instrument_id, amount_of_lots, ask.price))

                break

    if len(tasks) > 0:
        await asyncio.gather(*tasks)


async def _buy(client, account, instrument_id, amount_of_lots, price):
    price_decimal = quotation_to_decimal(price)

    logger.info(f"Buy {amount_of_lots} lots with price {price_decimal}")


async def _sell(client, account, instrument_id, amount_of_lots, price):
    price_decimal = quotation_to_decimal(price)

    logger.info(f"Sell {amount_of_lots} lots with price {price_decimal}")


async def _cancel_orders(client):
    logger.info(f"Cancel orders")


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
