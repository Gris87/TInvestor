import asyncio
import argparse
import datetime as dt
import logging
import sys

from datetime import datetime
from loguru import logger
from zoneinfo import ZoneInfo

from t_tech.invest import GetMaxLotsRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from t_tech.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings
from t_tech.invest.utils import decimal_to_quotation, quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


MOSCOW_TZ = ZoneInfo("Europe/Moscow")

WORKDAY_START = dt.time(10, 0, tzinfo=MOSCOW_TZ)
WORKDAY_END   = dt.time(23, 30, tzinfo=MOSCOW_TZ)

TMON_UID = "498ec3ff-ef27-4729-9703-a5aac48d5789"

QUANTITY_THRESHOLD = 5000000
BUY_PRICE_OFFSET = 2


async def tmon_weekend_trading(args):
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
    bid_found = False
    ask_found = False
    prices = []

    for bid in orderbook.bids:
        if bid.quantity >= QUANTITY_THRESHOLD:
            bid_found = True

            break

        prices.append(quotation_to_decimal(bid.price))

    for ask in orderbook.asks:
        if ask.quantity >= QUANTITY_THRESHOLD:
            ask_found = True

            break

        prices.append(quotation_to_decimal(ask.price))

    if bid_found and ask_found and len(prices) > 0:
        prices.sort(reverse=True)

        tasks = []

        if len(prices) > BUY_PRICE_OFFSET:
            tasks.append(_buy(client, account, decimal_to_quotation(prices[BUY_PRICE_OFFSET])))

        tasks.append(_sell(client, account, decimal_to_quotation(prices[0])))

        if len(tasks) > 0:
            await asyncio.gather(*tasks)


async def _buy(client, account, price):
    req = GetMaxLotsRequest(account_id=account, instrument_id=TMON_UID, price=price)
    max_lots = await client.orders.get_max_lots(req)

    amount_to_buy = max_lots.buy_limits.buy_max_lots

    if amount_to_buy > 0:
        resp = await client.orders.post_order(
            quantity=amount_to_buy,
            price=price,
            direction=OrderDirection.ORDER_DIRECTION_BUY,
            account_id=account,
            order_type=OrderType.ORDER_TYPE_LIMIT,
            instrument_id=TMON_UID,
            time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
            price_type=PriceType.PRICE_TYPE_CURRENCY
        )

        if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
            logger.info(f"Buy {amount_to_buy} lots with price {quotation_to_decimal(price)}")


async def _sell(client, account, price):
    req = GetMaxLotsRequest(account_id=account, instrument_id=TMON_UID, price=price)
    max_lots = await client.orders.get_max_lots(req)

    amount_to_sell = max_lots.sell_limits.sell_max_lots

    if amount_to_sell > 0:
        resp = await client.orders.post_order(
            quantity=amount_to_sell,
            price=price,
            direction=OrderDirection.ORDER_DIRECTION_SELL,
            account_id=account,
            order_type=OrderType.ORDER_TYPE_LIMIT,
            instrument_id=TMON_UID,
            time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
            price_type=PriceType.PRICE_TYPE_CURRENCY
        )

        if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
            logger.info(f"Sell {amount_to_sell} lots with price {quotation_to_decimal(price)}")


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
