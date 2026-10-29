import asyncio
import argparse
import datetime as dt
import logging
import requests
import sys
import time

from datetime import datetime, timedelta
from http import HTTPStatus
from loguru import logger
from pathlib import Path
from zoneinfo import ZoneInfo

from t_tech.invest import InstrumentIdType, GetOperationsByCursorRequest
from t_tech.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings
from t_tech.invest.schemas import OperationState

from localization import *


#logging.basicConfig(level=logging.DEBUG)


RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c"

PROFILE_URL   = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}"
TRADES_URL    = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}/instrument"
PORTFOLIO_URL = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}/portfolio/analytics"

MOSCOW_TZ = ZoneInfo("Europe/Moscow")

WORKDAY_START = dt.time(6, 0, tzinfo=MOSCOW_TZ)
WORKDAY_END   = dt.time(23, 50, tzinfo=MOSCOW_TZ)

SESSION_EXPIRATION_TIME = 18 * 60 * 60 # 18 hours


_instruments_cache = {}
_types_cache = {}
_companies_cache = {}
_sectors_cache = {}


async def follow(args):
    if args.official_dest and not args.confirm:
        answer = input("Are you sure to use official destination account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return

    logger.info("Connecting to server")

    dest_token = _get_token(args.dest_token, args.dest_token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10000)
    retry_settings2 = RetryClientSettings(use_retry=True, max_retry_attempt=3)

    async with AsyncRetryingClient(dest_token, settings=retry_settings, target=INVEST_GRPC_API if args.official_dest else INVEST_GRPC_API_SANDBOX) as dest_client:
        async with AsyncRetryingClient(dest_token, settings=retry_settings2, target=INVEST_GRPC_API if args.official_dest else INVEST_GRPC_API_SANDBOX) as tidy_client:
            logger.info("Verifying accounts")

            if not await _validate_profile(args.src_profile, "Source"):
                sys.exit(1)

                return

            if not await _validate_account(dest_client, args.dest_account, "Destination"):
                sys.exit(1)

                return

            await _start_follow_poll_mode(dest_client, tidy_client, args.src_profile, args.dest_account)


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


async def _validate_profile(profile_id, direction):
    resp = requests.get(PROFILE_URL.format(profile_id=profile_id))

    if resp.status_code != HTTPStatus.OK:
        logger.error(f'{direction} profile "{profile_id}" not found')

        return False

    resp_json = resp.json()
    nickname = resp_json["payload"]["nickname"]

    if resp_json["payload"]["status"] != "open":
        logger.error(f'{direction} profile "{profile_id}" ({nickname}) not openned')

        return False

    logger.info(f'{direction} profile "{profile_id}" ({nickname}) found')

    return True


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

    logger.info(f'{direction} account "{account_id}" ({accounts[account_id]}) found')

    return True


async def _start_follow_poll_mode(dest_client, tidy_client, src_profile, dest_account):
    logger.info("Start in poll mode")

    last_modified = -1

    while True:
        if not _is_work_time():
            _instruments_cache.clear()
            _types_cache.clear()
            _companies_cache.clear()
            _sectors_cache.clear()

            seconds = _seconds_to_next_morning() + 1

            logger.info(f"Sleep for {seconds}s because current time is not valid")
            await asyncio.sleep(seconds)

            continue

        src_timestamp = await _get_last_profile_trade_timestamp(tidy_client, src_profile)
        dest_timestamp = await _get_last_operation_timestamp(dest_client, dest_account)

        current_modified = src_timestamp + dest_timestamp

        if current_modified != last_modified:
            last_modified = current_modified

            src_portfolio = await _get_profile_portfolio(src_profile)
            dest_portfolio = await _get_valid_portfolio(dest_client, dest_account)

            await _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio)

        await asyncio.sleep(10)


async def _get_last_profile_trade_timestamp(client, profile_id):
    payload = await _request_profile_trades(profile_id)
    items = payload["items"]

    if len(_instruments_cache) == 0:
        logger.info("Filling instruments cache")

        await _fill_instruments_cache_from_payload(client, payload)

        while payload["hasNext"]:
            next_cursor = payload["nextCursor"]

            payload = await _request_profile_trades(profile_id, next_cursor)
            await _fill_instruments_cache_from_payload(client, payload)
    else:
        await _fill_instruments_cache_from_payload(client, payload)

    if len(items) > 0:
        last_trade = items[0]
        max_trade_date_time = last_trade["statistics"]["maxTradeDateTime"]

        return datetime.fromisoformat(max_trade_date_time).timestamp()

    return 0


async def _fill_instruments_cache_from_payload(client, payload):
    items = payload["items"]

    for item in items:
        if item["currency"] == "rub":
            class_code = item["classCode"]
            ticker = item["ticker"]

            full_id = class_code + "_" + ticker

            if full_id not in _instruments_cache:
                type_localized = TYPES_RUS[item["type"]]

                uid, lot, company, sector = await _get_instrument_info(client, class_code, ticker)

                _instruments_cache[full_id] = {
                    "uid": uid,
                    "lot": lot,
                }

                if type_localized not in _types_cache:
                    _types_cache[type_localized] = {}

                if company not in _companies_cache:
                    _companies_cache[company] = {}

                if sector not in _sectors_cache:
                    _sectors_cache[sector] = {}

                _types_cache[type_localized][full_id] = {}
                _companies_cache[company][full_id] = {}
                _sectors_cache[sector][full_id] = {}


async def _get_instrument_info(client, class_code, ticker):
    logger.info(f"Getting info for instrument {class_code}/{ticker}")

    resp1 = await client.instruments.get_instrument_by(id_type=InstrumentIdType.INSTRUMENT_ID_TYPE_TICKER, class_code=class_code, id=ticker)
    asset_uid = resp1.instrument.asset_uid

    company = OTHER
    sector = OTHER

    if asset_uid != "":
        resp2 = await client.instruments.get_asset_by(id=asset_uid)
        company = resp2.asset.brand.company
        sector = SECTORS_RUS[resp2.asset.brand.sector]

    return resp1.instrument.uid, resp1.instrument.lot, company, sector


async def _request_profile_trades(profile_id, next_cursor=""):
    next_cursor = f"?nextCursor={next_cursor}" if next_cursor != "" else ""

    while True:
        cookies = {"sessionID": _get_session_id()}

        resp = requests.get(TRADES_URL.format(profile_id=profile_id)+next_cursor, cookies=cookies)

        if resp.status_code != HTTPStatus.OK:
            await asyncio.sleep(1)

            continue

        resp_json = resp.json()

        return resp_json["payload"]


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


async def _get_profile_portfolio(profile_id):
    payload = await _request_profile_portfolio(profile_id)
    print(payload)

    return {}


async def _request_profile_portfolio(profile_id):
    while True:
        cookies = {"sessionID": _get_session_id()}

        resp = requests.get(PORTFOLIO_URL.format(profile_id=profile_id), cookies=cookies)

        if resp.status_code != HTTPStatus.OK:
            await asyncio.sleep(1)

            continue

        resp_json = resp.json()

        return resp_json["payload"]


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


async def _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio):
    print(src_portfolio)
    print(dest_portfolio)


def _get_session_id():
    session_path = Path("session_id.txt")

    first_warning = True

    while True:
        now = time.time()
        mtime = session_path.stat().st_mtime if session_path.is_file() else 0

        if now - mtime > SESSION_EXPIRATION_TIME:
            if first_warning:
                first_warning = False

                logger.warning("Waiting for new session ID")

            time.sleep(1)

            continue

        break

    with open(session_path, "r") as f:
        return f.read().strip()


def _is_work_time():
    d = datetime.now(MOSCOW_TZ)
    t = d.timetz()

    return t >= WORKDAY_START and t < WORKDAY_END


def _seconds_to_next_morning():
    now = datetime.now(MOSCOW_TZ)

    target_morning = now.replace(hour=WORKDAY_START.hour, minute=WORKDAY_START.minute, second=0, microsecond=0)

    if now >= target_morning:
        target_morning += timedelta(days=1)

    return int((target_morning - now).total_seconds())


def main():
    parser = argparse.ArgumentParser()
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
        "--dest-token",
        dest="dest_token",
        type=str,
        default="",
        help="Token for Tinkoff API (Destination account)",
    )
    parser.add_argument(
        "--dest-token-file",
        dest="dest_token_file",
        type=str,
        default="",
        help="Path to file with token for Tinkoff API (Destination account)",
    )
    parser.add_argument(
        "--src-profile",
        dest="src_profile",
        type=str,
        default="",
        help="Source Pulse profile UUID",
    )
    parser.add_argument(
        "--dest-account",
        dest="dest_account",
        type=str,
        default="",
        help="Destination account ID",
    )
    args = parser.parse_args()

    if (args.dest_token == "" and args.dest_token_file == "") or (args.dest_token != "" and args.dest_token_file != ""):
        logger.error("Please specify destination token with --dest-token or --dest-token-file")

        sys.exit(1)

    if args.src_profile == "":
        logger.error("Please specify source Pulse profile UUID with --src-profile")

        sys.exit(1)

    if args.dest_account == "":
        logger.error("Please specify destination account ID with --dest-account")

        sys.exit(1)

    asyncio.run(follow(args))
