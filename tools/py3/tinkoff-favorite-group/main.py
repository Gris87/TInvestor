import asyncio
import argparse
import logging
import sys
from loguru import logger

from tinkoff.invest.constants import INVEST_GRPC_API
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings


#logging.basicConfig(level=logging.DEBUG)


async def favorite_group(args):
    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API) as client:
        logger.info("Obtaining favorite groups")

    return


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


def main():
    parser = argparse.ArgumentParser()
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
    args = parser.parse_args()

    if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
        logger.error("Please specify token with --token or --token-file")

        sys.exit(1)

    asyncio.run(favorite_group(args))
