import argparse
import sys
from loguru import logger

from tinkoff.invest import Client
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX


def follow(args):
    if args.real_dest and not args.confirm:
        answer = input("Are you sure to use real destination account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return True

    logger.info("Connecting to server")

    with Client(args.src_token, target=INVEST_GRPC_API if args.real_src else INVEST_GRPC_API_SANDBOX) as src_client:
        with Client(args.dest_token, target=INVEST_GRPC_API if args.real_dest else INVEST_GRPC_API_SANDBOX) as dest_client:
            logger.info("Verifying accounts")

            if not _validate_account(src_client, args.src_account, "Source"):
                return False

            if not _validate_account(dest_client, args.dest_account, "Destination"):
                return False

    return True


def _validate_account(client, account_id, direction):
    accounts = {}

    for account in client.users.get_accounts().accounts:
        accounts[account.id] = account.name

    if account_id not in accounts:
        logger.error(f'{direction} account "{account_id}" not found. Please use one of the following')

        for id, name in accounts.items():
            print(f"{id:36} | {name}")

        return False

    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--real-src",
        dest="real_src",
        default=False,
        action="store_true",
        help="Flag for using official server for source account",
    )
    parser.add_argument(
        "--real-dest",
        dest="real_dest",
        default=False,
        action="store_true",
        help="Flag for using official server for destination account",
    )
    parser.add_argument(
        "--confirm",
        dest="confirm",
        default=False,
        action="store_true",
        help="Do not ask about using real destination account",
    )
    parser.add_argument(
        "--src-token",
        dest="src_token",
        type=str,
        default="t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ",
        help="Token for Tinkoff API (Source account)",
    )
    parser.add_argument(
        "--dest-token",
        dest="dest_token",
        type=str,
        default="t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ",
        help="Token for Tinkoff API (Destination account)",
    )
    parser.add_argument(
        "--src-account",
        dest="src_account",
        type=str,
        default="8963c33d-7ee2-4ff2-ac01-8c090ec502f5",
        help="Source account id",
    )
    parser.add_argument(
        "--dest-account",
        dest="dest_account",
        type=str,
        default="d1843f24-0864-4666-8608-d5d16822fbae",
        help="Destination account id",
    )
    args = parser.parse_args()

    sys.exit(0 if follow(args) else 1)
