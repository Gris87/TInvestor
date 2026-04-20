import asyncio
import argparse
import logging
import os
import sys

from loguru import logger

from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings


#logging.basicConfig(level=logging.DEBUG)


async def asap_trading(args):
    logger.info("Connecting to server")

    token = os.environ["TINVEST_TOKEN"]
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API if args.official else INVEST_GRPC_API_SANDBOX) as client:
        await _do_processing(client, args.account, args.instrument_id)


async def _do_processing(client, account_id, instrument_id):
    while True:
        portfolio = await client.operations.get_portfolio(account_id=account_id)

        found = False

        for position in portfolio.positions:
            if position.instrument_uid==instrument_id:
                found = True

                break

        if not found:
            break

        await _do_instrument_processing(client, account_id, instrument_id)

        await asyncio.sleep(1000) # TODO: 1


async def _do_instrument_processing(client, account_id, instrument_id):
    print(account_id)
    print(instrument_id)


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
    args = parser.parse_args()

    if args.account == "":
        logger.error("Please specify account ID with --account")

        sys.exit(1)

    if args.instrument_id == "":
        logger.error("Please specify instrument ID with --instrument-id")

        sys.exit(1)

    asyncio.run(asap_trading(args))


if __name__ == "__main__":
    sys.exit(main())
