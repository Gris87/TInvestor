import asyncio
import argparse
import datetime as dt
import json
import logging
import requests
import sys
import time
import traceback

from datetime import datetime, timedelta
from decimal import Decimal
from http import HTTPStatus
from loguru import logger
from pathlib import Path
from zoneinfo import ZoneInfo

from t_tech.invest import InstrumentIdType, GetMaxLotsRequest, GetOperationsByCursorRequest, MoneyValue, OrderDirection, OrderExecutionReportStatus, OrderType, PortfolioPosition, PortfolioResponse, PriceType, TimeInForceType
from t_tech.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from t_tech.invest.exceptions import AioRequestError
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings
from t_tech.invest.schemas import OperationState, OrderIdType
from t_tech.invest.utils import decimal_to_money, decimal_to_quotation, money_to_decimal, quotation_to_decimal

from localization import *


#logging.basicConfig(level=logging.DEBUG)


RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c"

PROFILE_URL   = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}"
TRADES_URL    = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}/instrument"
PORTFOLIO_URL = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}/portfolio/analytics"

MOSCOW_TZ = ZoneInfo("Europe/Moscow")

WORKDAY_START = dt.time(6, 0, tzinfo=MOSCOW_TZ)
WORKDAY_END   = dt.time(23, 50, tzinfo=MOSCOW_TZ)

SESSION_EXPIRATION_TIME = 23 * 60 * 60 # 23 hours

IGNORE_INSTRUMENTS = {
    "PSSU/DIAS",
    "PSSU/UGLD",
    "TQBR/DIOD",
}

TOTAL_MONEY_AVAILABLE = Decimal("100000000.00")


_instruments_by_ticker_cache = {}
_instruments_by_uid_cache = {}
_last_complete_timestamp = -1
_total_operations = {}
_last_portfolio = {
    "types": {
        CURRENCY: 100
    },
    "issuers": {
        CURRENCY: 100
    },
    "sectors": {
        CURRENCY: 100
    }
}
_generated_portfolio = {}


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

            await _start_follow_poll_mode(dest_client, tidy_client, args.cache, args.src_profile, args.dest_account)


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


async def _start_follow_poll_mode(dest_client, tidy_client, cache_path, src_profile, dest_account):
    logger.info("Start in poll mode")

    _load_cache(cache_path)

    last_modified = -1

    while True:
        # logger.debug("Tick")

        if not _is_work_time():
            _instruments_by_ticker_cache.clear()
            _instruments_by_uid_cache.clear()

            seconds = _seconds_to_next_morning() + 1

            logger.info(f"Sleep for {seconds}s because current time is not valid")
            await asyncio.sleep(seconds)

            continue

        src_timestamp = await _get_last_profile_trade_timestamp(tidy_client, cache_path, src_profile)
        dest_timestamp = await _get_last_operation_timestamp(dest_client, dest_account)

        if last_modified != dest_timestamp or src_timestamp < 0:
            last_modified = dest_timestamp

            src_portfolio = await _get_profile_portfolio(tidy_client, cache_path, src_profile)
            dest_portfolio = await _get_valid_portfolio(dest_client, dest_account)

            await _handle_portfolios(dest_client, dest_account, src_portfolio, dest_portfolio)

        await asyncio.sleep(10)


async def _get_last_profile_trade_timestamp(client, cache_path, profile_id):
    global _last_complete_timestamp

    payload = await _request_profile_trades(profile_id)
    items = payload["items"]

    if len(_instruments_by_ticker_cache) == 0:
        logger.info("Filling instruments cache")

        await _fill_instruments_cache_from_items(client, payload["items"])

        while payload["hasNext"]:
            next_cursor = payload["nextCursor"]

            payload = await _request_profile_trades(profile_id, next_cursor)
            await _fill_instruments_cache_from_items(client, payload["items"])
    else:
        await _fill_instruments_cache_from_items(client, payload["items"])

    _save_instruments_cache(cache_path)

    if len(items) > 0:
        last_trade = items[0]

        class_code = last_trade["classCode"]
        ticker = last_trade["ticker"]

        full_id = class_code + "/" + ticker

        if full_id not in _total_operations or _total_operations[full_id] != last_trade["statistics"]["totalOperationsCount"]:
            # logger.debug("New operation found")

            if _last_complete_timestamp < 0:
                _total_operations[full_id] = last_trade["statistics"]["totalOperationsCount"]
                max_trade_date_time = last_trade["statistics"]["maxTradeDateTime"]

                _last_complete_timestamp = datetime.fromisoformat(max_trade_date_time).timestamp()

            return -1

        payload = await _request_profile_portfolio(profile_id)
        payload = await _simplify_portfolio(payload)

        if _last_portfolio != payload:
            # logger.debug("Portfolio change found")

            return -1

    return 0


async def _fill_instruments_cache_from_items(client, items):
    for item in items:
        if item["currency"] == "rub":
            class_code = item["classCode"]
            ticker = item["ticker"]

            full_id = class_code + "/" + ticker

            if full_id not in _instruments_by_ticker_cache and full_id not in IGNORE_INSTRUMENTS:
                logger.info(f"Getting info for instrument {full_id}")

                while True:
                    try:
                        instrument_type, uid, lot, nominal, company, sector = await _get_instrument_info(client, item["type"], class_code, ticker)

                        _instruments_by_ticker_cache[full_id] = {
                            "type": instrument_type,
                            "uid": uid,
                            "lot": lot,
                            "nominal": nominal,
                            "company": company,
                            "sector": sector,
                        }
                        _instruments_by_uid_cache[uid] = {
                            "full_id": full_id,
                            "lot": lot,
                            "nominal": nominal,
                        }
                    except AioRequestError as e:
                        logger.warning(f"Failed to get info for instrument {full_id}: {e}")

                        if e.details != "50002": # NOT_FOUND (instrument not found)
                            logger.warning("Retrying")

                            continue

                    break


async def _get_instrument_info(client, instrument_type, class_code, ticker):
    resp1 = await client.instruments.get_instrument_by(id_type=InstrumentIdType.INSTRUMENT_ID_TYPE_TICKER, class_code=class_code, id=ticker)

    if not resp1.instrument.api_trade_available_flag:
        return "", "", 0, "1.00", "", ""

    nominal = "1.00"
    company = OTHER
    sector = OTHER

    if resp1.instrument.asset_uid != "":
        resp2 = await client.instruments.get_asset_by(id=resp1.instrument.asset_uid)
        company = resp2.asset.brand.company
        sector = SECTORS_RUS[resp2.asset.brand.sector.replace("_", "").lower()]

    if instrument_type == "bond":
        resp3 = await client.instruments.bond_by(id_type=InstrumentIdType.INSTRUMENT_ID_TYPE_TICKER, class_code=class_code, id=ticker)

        nominal_decimal = money_to_decimal(resp3.instrument.nominal) / Decimal("100.00")
        nominal=str(nominal_decimal)
    elif instrument_type == "futures":
        resp3 = await client.instruments.future_by(id_type=InstrumentIdType.INSTRUMENT_ID_TYPE_TICKER, class_code=class_code, id=ticker)

        if resp3.instrument.sector != "":
            sector = SECTORS_RUS[resp3.instrument.sector.replace("SECTOR_", "").replace("_", "").lower()]

        try:
            if resp3.instrument.basic_asset_position_uid != "":
                resp4 = await client.instruments.get_instrument_by(id_type=InstrumentIdType.INSTRUMENT_ID_TYPE_POSITION_UID, id=resp3.instrument.basic_asset_position_uid)

                if resp4.instrument.currency != "rub":
                    instrument_type = "neoactive"

                if resp4.instrument.asset_uid != "":
                    resp5 = await client.instruments.get_asset_by(id=resp4.instrument.asset_uid)
                    company = resp5.asset.brand.company
                    sector = SECTORS_RUS[resp5.asset.brand.sector.replace("_", "").lower()]
        except AioRequestError as e:
            logger.warning(f"Failed to get info for instrument from position uid {resp3.instrument.basic_asset_position_uid}: {e}")

    return TYPES_RUS[instrument_type], resp1.instrument.uid, resp1.instrument.lot, nominal, company, sector


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
    try:
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
    except Exception as e:
        logger.warning(f"Failed to get last operation timestamp: {e}")
        logger.warning(traceback.format_exc())

    return 0


async def _get_profile_portfolio(client, cache_path, profile_id):
    global _last_complete_timestamp
    global _last_portfolio
    global _generated_portfolio

    payload = await _request_profile_portfolio(profile_id)
    await _validate_constants(payload)

    while True:
        trades = await _get_trades_from_timestamp(profile_id, _last_complete_timestamp)

        payload_to_double_check = await _request_profile_portfolio(profile_id)

        if payload != payload_to_double_check:
            payload = payload_to_double_check

            continue

        break

    if len(trades) > 0:
        payload = await _simplify_portfolio(payload)

        logger.debug("New trades found:\n{trades}".format(trades=json.dumps(trades, indent=4, ensure_ascii=False)))
        logger.debug("Portfolio changed from:\n{old_portfolio}\n\nto:\n\n{new_portfolio}".format(old_portfolio=json.dumps(_last_portfolio, indent=4, ensure_ascii=False), new_portfolio=json.dumps(payload, indent=4, ensure_ascii=False)))

        await _fill_instruments_cache_from_items(client, trades)
        _generated_portfolio = await _generate_portfolio(_generated_portfolio, _last_portfolio, payload, trades)

        last_trade = trades[0]
        max_trade_date_time = last_trade["statistics"]["maxTradeDateTime"]

        _last_complete_timestamp = datetime.fromisoformat(max_trade_date_time).timestamp()
        _last_portfolio = payload

        _save_portfolio_cache(cache_path)

    res = await _convert_generated_portfolio(_generated_portfolio)

    return res


async def _validate_constants(payload):
    for t in payload["types"]:
        title = t["title"]

        if title not in TYPES_VALIDATION:
            logger.warning(f'Type "{title}" is unknown')

    for s in payload["sectors"]:
        title = s["title"]

        if title not in SECTORS_VALIDATION:
            logger.warning(f'Sector "{title}" is unknown')


async def _get_trades_from_timestamp(profile_id, timestamp):
    res = []

    payload = await _request_profile_trades(profile_id)

    while True:
        for trade in payload["items"]:
            max_trade_date_time = trade["statistics"]["maxTradeDateTime"]
            trade_timestamp = datetime.fromisoformat(max_trade_date_time).timestamp()

            if trade_timestamp < timestamp:
                return res

            if trade["currency"] == "rub":
                class_code = trade["classCode"]
                ticker = trade["ticker"]

                full_id = class_code + "/" + ticker

                _total_operations[full_id] = trade["statistics"]["totalOperationsCount"]

                res.append(trade)

        if not payload["hasNext"]:
            break

        next_cursor = payload["nextCursor"]
        payload = await _request_profile_trades(profile_id, next_cursor)

    return res


async def _simplify_portfolio(portfolio):
    return {
        "types": {x["title"]: x["percent"] for x in portfolio["types"] if x["title"] == CURRENCY or x["percent"] != 0},
        "issuers": {x["title"]: x["percent"] for x in portfolio["issuers"] if x["title"] == CURRENCY or x["percent"] != 0},
        "sectors": {x["title"]: x["percent"] for x in portfolio["sectors"] if x["title"] == CURRENCY or x["percent"] != 0},
    }


async def _generate_portfolio(generated_portfolio, old_portfolio, new_portfolio, trades):
    for full_id in list(generated_portfolio.keys()):
        part = await _calculate_part(new_portfolio, full_id)

        if part == 0:
            del generated_portfolio[full_id]

    for trade in trades:
        class_code = trade["classCode"]
        ticker = trade["ticker"]

        full_id = class_code + "/" + ticker

        new_part = await _calculate_part(new_portfolio, full_id)

        if new_part != 0:
            old_part = await _calculate_part(old_portfolio, full_id)

            if full_id not in generated_portfolio:
                generated_portfolio[full_id] = 0

            generated_portfolio[full_id] += new_part - old_part

    return generated_portfolio


async def _calculate_part(portfolio, full_id):
    if full_id not in _instruments_by_ticker_cache:
        return 0

    instrument_info = _instruments_by_ticker_cache[full_id]

    instrument_type = instrument_info["type"]
    company = instrument_info["company"]
    sector = instrument_info["sector"]

    type_part = portfolio["types"].get(instrument_type, 0)
    company_part = portfolio["issuers"].get(company, 0)
    sector_part = portfolio["sectors"].get(sector, 0)

    return min(type_part, company_part, sector_part)


async def _convert_generated_portfolio(generated_portfolio):
    positions = []

    money_remained = TOTAL_MONEY_AVAILABLE

    for full_id, part in generated_portfolio.items():
        instrument_info = _instruments_by_ticker_cache[full_id]

        instrument_uid = instrument_info["uid"]
        lot = instrument_info["lot"]

        money_part = TOTAL_MONEY_AVAILABLE * Decimal(str(part / 100))
        money_remained -= money_part

        current_price = Decimal("1.000000000") / lot

        positions.append(PortfolioPosition(
            instrument_uid=instrument_uid,
            quantity=decimal_to_quotation(money_part),
            average_position_price=MoneyValue(currency="rub", units=1, nano=0),
            current_price=decimal_to_money(current_price, currency="rub")
        ))

    positions.insert(0, PortfolioPosition(
        instrument_uid=RUBLE_UID,
        quantity=decimal_to_quotation(money_remained),
    ))

    return PortfolioResponse(positions=positions)


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
        try:
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
        except Exception as e:
            logger.warning(f"Failed to get valid portfolio: {e}")
            logger.warning(traceback.format_exc())

        await asyncio.sleep(1)


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

        instrument_info = _instruments_by_uid_cache[src_instrument_id]
        lot = instrument_info["lot"]

        dest_portfolio_item = dest_instrument_to_cost[src_instrument_id]
        delta = expected_cost - dest_portfolio_item["cost"]
        lot_price = dest_portfolio_item["price"] * lot

        if delta < -lot_price / 2:
            instruments_for_sale[src_instrument_id] = expected_cost
        elif delta > lot_price / 2:
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
    instrument_info = _instruments_by_uid_cache[instrument_id]
    full_id = instrument_info["full_id"]
    lot = instrument_info["lot"]
    nominal = Decimal(instrument_info["nominal"])

    order_id = None
    order_price = None

    while True:
        try:
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

            if price is None:
                break

            price = decimal_to_quotation(quotation_to_decimal(price) * nominal)

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
                amount_to_buy = min(delta_quantity, max_lots.buy_margin_limits.buy_max_lots)

                if amount_to_buy > 0:
                    logger.debug(f"Creating order to buy {amount_to_buy} lots of {full_id} at price {quotation_to_decimal(price)}")

                    resp = await dest_client.orders.post_order(
                        quantity=amount_to_buy,
                        price=price,
                        direction=OrderDirection.ORDER_DIRECTION_BUY,
                        account_id=dest_account,
                        order_type=OrderType.ORDER_TYPE_LIMIT,
                        instrument_id=instrument_id,
                        time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                        price_type=PriceType.PRICE_TYPE_CURRENCY,
                        confirm_margin_trade=True,
                    )

                    if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                        order_id = resp.order_id
                        order_price = price
                    else:
                        logger.warning(f'Order to buy "{instrument_id}" rejected')
                else:
                    break
        except Exception as e:
            logger.warning(f"Failed to buy: {e}")
            logger.warning(traceback.format_exc())

        await asyncio.sleep(30)


async def _sell(dest_client, dest_account, instrument_id, delta, sell_all):
    instrument_info = _instruments_by_uid_cache[instrument_id]
    full_id = instrument_info["full_id"]
    lot = instrument_info["lot"]
    nominal = Decimal(instrument_info["nominal"])

    order_id = None
    order_price = None

    while True:
        try:
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

            if price is None:
                break

            price = decimal_to_quotation(quotation_to_decimal(price) * nominal)

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
                    amount_to_sell = min(delta_quantity, max_lots.sell_margin_limits.sell_max_lots)

                if amount_to_sell > 0:
                    logger.debug(f"Creating order to sell {amount_to_sell} lots of {full_id} at price {quotation_to_decimal(price)}")

                    resp = await dest_client.orders.post_order(
                        quantity=amount_to_sell,
                        price=price,
                        direction=OrderDirection.ORDER_DIRECTION_SELL,
                        account_id=dest_account,
                        order_type=OrderType.ORDER_TYPE_LIMIT,
                        instrument_id=instrument_id,
                        time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                        price_type=PriceType.PRICE_TYPE_CURRENCY,
                        confirm_margin_trade=True,
                    )

                    if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                        order_id = resp.order_id
                        order_price = price
                    else:
                        logger.warning(f'Order to sell "{instrument_id}" rejected')
                else:
                    break
        except Exception as e:
            logger.warning(f"Failed to sell: {e}")
            logger.warning(traceback.format_exc())

        await asyncio.sleep(30)


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


def _load_cache(cache_path):
    global _instruments_by_ticker_cache
    global _last_complete_timestamp
    global _total_operations
    global _last_portfolio
    global _generated_portfolio

    cache_folder_path = Path(cache_path) / "tinkoff_pulse_follower"
    instruments_path = cache_folder_path / "instruments.json"
    portfolio_path = cache_folder_path / "portfolio.json"

    if instruments_path.exists():
        with open(instruments_path, "r", encoding="utf-8") as f:
            _instruments_by_ticker_cache = json.loads(f.read())

            _instruments_by_uid_cache.clear()

            for full_id, instrument_info in _instruments_by_ticker_cache.items():
                uid = instrument_info["uid"]
                lot = instrument_info["lot"]
                nominal = instrument_info["nominal"]

                _instruments_by_uid_cache[uid] = {
                    "full_id": full_id,
                    "lot": lot,
                    "nominal": nominal
                }

    if portfolio_path.exists():
        with open(portfolio_path, "r", encoding="utf-8") as f:
            data = json.loads(f.read())

            _last_complete_timestamp = data["timestamp"]
            _total_operations = data["total_operations"]
            _last_portfolio = data["last_portfolio"]
            _generated_portfolio = data["generated"]


def _save_instruments_cache(cache_path):
    cache_folder_path = Path(cache_path) / "tinkoff_pulse_follower"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    instruments_path = cache_folder_path / "instruments.json"

    with open(instruments_path, "w", encoding="utf-8") as f:
        json.dump(_instruments_by_ticker_cache, f, ensure_ascii=False)


def _save_portfolio_cache(cache_path):
    cache_folder_path = Path(cache_path) / "tinkoff_pulse_follower"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    portfolio_path = cache_folder_path / "portfolio.json"

    data = {
        "timestamp": _last_complete_timestamp,
        "total_operations": _total_operations,
        "last_portfolio": _last_portfolio,
        "generated": _generated_portfolio,
    }

    with open(portfolio_path, "w", encoding="utf-8") as f:
        json.dump(data, f, ensure_ascii=False)


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
    parser.add_argument(
        "--cache",
        dest="cache",
        type=str,
        default="",
        help="Path to cache folder"
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

    if args.cache == "":
        logger.error("Please specify path to cache folder with --cache")

        sys.exit(1)

    asyncio.run(follow(args))
