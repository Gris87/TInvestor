import asyncio
import argparse
import datetime as dt
import logging
import sys

from datetime import datetime
from decimal import Decimal
from loguru import logger
from zoneinfo import ZoneInfo

from tinkoff.invest import GetMaxLotsRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings
from tinkoff.invest.schemas import OrderIdType
from tinkoff.invest.utils import decimal_to_quotation, quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


MOSCOW_TZ = ZoneInfo("Europe/Moscow")

WORKDAY_START = dt.time(10, 0, tzinfo=MOSCOW_TZ)
WORKDAY_END   = dt.time(23, 30, tzinfo=MOSCOW_TZ)

TMON_UID = "498ec3ff-ef27-4729-9703-a5aac48d5789"


async def tmon_weekend_trading(args):
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
            sys.exit(1)

            return

        await _start_orderbook_streaming(client, args.account)


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


async def _start_orderbook_streaming(client, account):
    while True:
        if not _is_weekend_work_time():
            logger.info("Terminate because current time is not valid")

            break

        orderbook = await client.market_data.get_order_book(instrument_id=TMON_UID, depth=50)
        await _handle_orderbook(client, account, orderbook)

        await asyncio.sleep(10)


async def _handle_orderbook(client, account, orderbook):
    print("aaaaaa")


def _is_weekend_work_time():
    d = datetime.now(MOSCOW_TZ)

    if d.isoweekday() < 6:
        return False

    t = d.timetz()

    return t >= WORKDAY_START and t < WORKDAY_END


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
    args = parser.parse_args()

    if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
        logger.error("Please specify token with --token or --token-file")

        sys.exit(1)

    if args.account == "":
        logger.error("Please specify account ID with --account")

        sys.exit(1)

    asyncio.run(tmon_weekend_trading(args))
