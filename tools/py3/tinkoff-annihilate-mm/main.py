import asyncio
import argparse
import logging
import sys

from decimal import Decimal, ROUND_CEILING
from loguru import logger

from t_tech.invest import GetMaxLotsRequest, OrderDirection, OrderExecutionReportStatus, OrderType, PriceType, TimeInForceType
from t_tech.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings
from t_tech.invest.utils import quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c"
TMON_UID  = "498ec3ff-ef27-4729-9703-a5aac48d5789"

FREE_MM_MONEY = 5000.0


async def annihilate_mm(args):
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

        money, tmon_cost = await _get_money_and_tmon_cost(client, args.account)
        logger.info(f"You have {money} money while TMON cost {tmon_cost}")

        if money >= Decimal(-FREE_MM_MONEY) or tmon_cost <= Decimal(0):
            logger.info("There is nothing to do")

            return

        await _sell_tmon(client, args.account, -Decimal(FREE_MM_MONEY) - money)


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


async def _get_money_and_tmon_cost(client, account_id):
    money = Decimal(0)
    tmon_cost = Decimal(0)

    while True:
        portfolio = await client.operations.get_portfolio(account_id=account_id)

        good = True

        for position in portfolio.positions:
            if position.instrument_uid==RUBLE_UID:
                money = quotation_to_decimal(position.quantity)
            elif position.instrument_uid==TMON_UID:
                good = (position.average_position_price.units > 0 or position.average_position_price.nano > 0)

                if good:
                    tmon_cost = quotation_to_decimal(position.quantity) * quotation_to_decimal(position.average_position_price)

        if good:
            break

        await asyncio.sleep(1)

    return money, tmon_cost


async def _sell_tmon(client, account_id, cost):
    logger.info(f"Selling TMON with cost {cost}")

    orderbook = await client.market_data.get_order_book(instrument_id=TMON_UID, depth=1)

    if len(orderbook.bids) <= 0:
        logger.warning(f"Impossible to sell TMON")

        return

    price = orderbook.bids[0].price
    amount_of_lots_decimal = cost / quotation_to_decimal(price)
    amount_of_lots = int(amount_of_lots_decimal.to_integral_value(rounding=ROUND_CEILING))

    while True:
        req = GetMaxLotsRequest(account_id=account_id, instrument_id=TMON_UID, price=price)
        max_lots = await client.orders.get_max_lots(req)

        amount_to_sell = min(amount_of_lots, max_lots.sell_limits.sell_max_lots)

        if amount_to_sell > 0:
            resp = await client.orders.post_order(
                quantity=amount_to_sell,
                price=price,
                direction=OrderDirection.ORDER_DIRECTION_SELL,
                account_id=account_id,
                order_type=OrderType.ORDER_TYPE_LIMIT,
                instrument_id=TMON_UID,
                time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
                price_type=PriceType.PRICE_TYPE_CURRENCY
            )

            if resp.execution_report_status != OrderExecutionReportStatus.EXECUTION_REPORT_STATUS_REJECTED:
                logger.info(f"Sell {amount_to_sell} lots with price {quotation_to_decimal(price)}")

                break
        else:
            break

        await asyncio.sleep(1)


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

    asyncio.run(annihilate_mm(args))
