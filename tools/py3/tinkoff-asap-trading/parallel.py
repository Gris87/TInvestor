import asyncio
import argparse
import logging
import os
import sys

from decimal import Decimal
from loguru import logger

from tinkoff.invest import OrderDirection, OrderType, PriceType, TimeInForceType
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings
from tinkoff.invest.schemas import OrderIdType
from tinkoff.invest.utils import quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


HUNDRED_PERCENT = 100


async def asap_trading(args):
    logger.info("Connecting to server")

    token = os.environ["TINVEST_TOKEN"]
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10000)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API if args.official else INVEST_GRPC_API_SANDBOX) as client:
        await _do_processing(args, client)


async def _do_processing(args, client):
    logger.info(f"Starting processing for instrument {args.instrument_id}")

    while True:
        portfolio = await client.operations.get_portfolio(account_id=args.account)

        amount_of_lots = 0

        for position in portfolio.positions:
            if position.instrument_uid==args.instrument_id:
                amount_of_lots = int(quotation_to_decimal(position.quantity_lots))

                if position.average_position_price.units > 0 or position.average_position_price.nano > 0:
                    avg_price = quotation_to_decimal(position.average_position_price)

                    await _do_instrument_processing(args, client, amount_of_lots, avg_price)

                break

        if amount_of_lots == 0:
            break

        await asyncio.sleep(1)


async def _do_instrument_processing(args, client, amount_of_lots, avg_price):
    orderbook = await client.market_data.get_order_book(instrument_id=args.instrument_id, depth=50)

    if amount_of_lots > 0:
        await _sell(args, client, orderbook, amount_of_lots, avg_price)
    else:
        await _buy(args, client, orderbook, -amount_of_lots, avg_price)


async def _buy(args, client, orderbook, amount_of_lots, avg_price):
    if args.lose < 0 and len(orderbook.bids) > 0 and len(orderbook.asks) > 0:
        bid = orderbook.bids[0]
        ask = orderbook.asks[0]
        limit_price = avg_price / Decimal((HUNDRED_PERCENT + args.lose) / HUNDRED_PERCENT)

        if quotation_to_decimal(bid.price) >= limit_price:
            await _post_order(args, client, OrderDirection.ORDER_DIRECTION_BUY, amount_of_lots, ask.price)

            return

    min_lots_to_trade = min(round(amount_of_lots * args.part / HUNDRED_PERCENT), amount_of_lots)
    limit_price = avg_price / Decimal((HUNDRED_PERCENT + args.yield_value) / HUNDRED_PERCENT)

    lots = 0

    for ask in orderbook.asks:
        if quotation_to_decimal(ask.price) > limit_price:
            return

        lots += ask.quantity

        if lots >= min_lots_to_trade:
            await _post_order(args, client, OrderDirection.ORDER_DIRECTION_BUY, min(amount_of_lots, lots), ask.price)

            return


async def _sell(args, client, orderbook, amount_of_lots, avg_price):
    if args.lose < 0 and len(orderbook.bids) > 0 and len(orderbook.asks) > 0:
        bid = orderbook.bids[0]
        ask = orderbook.asks[0]
        limit_price = avg_price * Decimal((HUNDRED_PERCENT + args.lose) / HUNDRED_PERCENT)

        if quotation_to_decimal(ask.price) <= limit_price:
            await _post_order(args, client, OrderDirection.ORDER_DIRECTION_SELL, amount_of_lots, bid.price)

            return

    min_lots_to_trade = min(round(amount_of_lots * args.part / HUNDRED_PERCENT), amount_of_lots)
    limit_price = avg_price * Decimal((HUNDRED_PERCENT + args.yield_value) / HUNDRED_PERCENT)

    lots = 0

    for bid in orderbook.bids:
        if quotation_to_decimal(bid.price) < limit_price:
            return

        lots += bid.quantity

        if lots >= min_lots_to_trade:
            await _post_order(args, client, OrderDirection.ORDER_DIRECTION_SELL, min(amount_of_lots, lots), bid.price)

            return


async def _post_order(args, client, direction, amount_of_lots, price):
    await _cancel_orders_for_instrument(args, client)

    if direction == OrderDirection.ORDER_DIRECTION_BUY:
        logger.info(f"Creating order to buy {amount_of_lots} at price {price}")
    else:
        logger.info(f"Creating order to sell {amount_of_lots} at price {price}")

    await client.orders.post_order(
        quantity=amount_of_lots,
        price=price,
        direction=direction,
        account_id=args.account,
        order_type=OrderType.ORDER_TYPE_LIMIT,
        instrument_id=args.instrument_id,
        time_in_force=TimeInForceType.TIME_IN_FORCE_DAY,
        price_type=PriceType.PRICE_TYPE_CURRENCY
    )


async def _cancel_orders_for_instrument(args, client):
    logger.info("Cancelling orders if exists")

    cancelled = False

    tinkoff_orders = await client.orders.get_orders(account_id=args.account)

    for order in tinkoff_orders.orders:
        if order.instrument_uid == args.instrument_id:
            await client.orders.cancel_order(
                account_id=args.account,
                order_id=order.order_id,
                order_id_type=OrderIdType.ORDER_ID_TYPE_EXCHANGE
            )

            cancelled = True

    if cancelled:
        await asyncio.sleep(3)


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
        "--lose",
        dest="lose",
        type=float,
        default=0.0,
        help="Trade instrument when bad yield reached (0.0 to disable)",
    )
    parser.add_argument(
        "--yield",
        dest="yield_value",
        type=float,
        default=0.7,
        help="Trade instrument when good yield reached",
    )
    parser.add_argument(
        "--part",
        dest="part",
        type=float,
        default=25.0,
        help="Allow to trade partially",
    )
    args = parser.parse_args()

    asyncio.run(asap_trading(args))


if __name__ == "__main__":
    sys.exit(main())
