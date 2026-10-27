import asyncio
import argparse
import logging
import requests
import sys

from http import HTTPStatus
from loguru import logger

from t_tech.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings


#logging.basicConfig(level=logging.DEBUG)


PROFILE_URL = "https://api-invest.tbank.ru/invest-terminal/api-social/social/v1/profile/{profile_id}"


async def follow(args):
    if args.official_dest and not args.confirm:
        answer = input("Are you sure to use official destination account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return

    logger.info("Connecting to server")

    dest_token = _get_token(args.dest_token, args.dest_token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10000)

    async with AsyncRetryingClient(dest_token, settings=retry_settings, target=INVEST_GRPC_API if args.official_dest else INVEST_GRPC_API_SANDBOX) as dest_client:
        logger.info("Verifying accounts")

        if not await _validate_profile(args.src_profile, "Source"):
            sys.exit(1)

            return

        if not await _validate_account(dest_client, args.dest_account, "Destination"):
            sys.exit(1)

            return


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
