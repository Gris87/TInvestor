import asyncio
import argparse
import logging
import os
import psutil
import signal
import subprocess
import sys

from loguru import logger
from pathlib import Path

from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings


#logging.basicConfig(level=logging.DEBUG)


PATH_TO_SCRIPT = Path(__file__).parent

TMON_UID = "498ec3ff-ef27-4729-9703-a5aac48d5789"

IGNORE_STOCKS = [
    TMON_UID
]

terminated = False


async def asap_trading(args):
    if args.official and not args.confirm:
        answer = input("Are you sure to use official account? [Y/n]")

        if answer != "" and answer != "Y" and answer != "y":
            return

    _terminate_all_children_processes(args)

    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API if args.official else INVEST_GRPC_API_SANDBOX) as client:
        logger.info("Verifying account")

        if not await _validate_account(client, args.account):
            return

        await _do_processing(args, token, client)

    _terminate_all_children_processes(args)


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


async def _do_processing(args, token, client):
    global terminated

    signal.signal(signal.SIGINT, _exit_gracefully)
    signal.signal(signal.SIGTERM, _exit_gracefully)

    while not terminated:
        portfolio = await client.operations.get_portfolio(account_id=args.account)

        for position in portfolio.positions:
            if position.instrument_type=="currency":
                continue

            if position.instrument_uid in IGNORE_STOCKS:
                continue

            _start_instrument_processing(args, token, position.instrument_uid, position.ticker)

        await asyncio.sleep(10)


def _start_instrument_processing(args, token, instrument_id, ticker):
    found = False

    for p in psutil.process_iter(["name", "cmdline"]):
        if "python" in p.info["name"]:
            cmdline = p.info["cmdline"]

            if cmdline is not None and len(cmdline) > 1 and "tools/py3/tinkoff-asap-trading/parallel.py" in cmdline[1] and args.account in cmdline and instrument_id in cmdline:
                found = True

    if found:
        return

    logger.info(f"Starting child process for instrument {instrument_id} ({ticker})")

    cmd = [
        "python",
        str(Path(PATH_TO_SCRIPT) / "parallel.py"),
        "--account", args.account,
        "--instrument-id", instrument_id,
        "--lose", str(args.lose),
        "--yield", str(args.yield_value),
        "--part", str(args.part)
    ]

    if args.official:
        cmd.append("--official")

    my_env = os.environ.copy()
    my_env["TINVEST_TOKEN"] = token

    subprocess.Popen(
        cmd,
        start_new_session=True,
        stdout=subprocess.DEVNULL,
        stderr=subprocess.DEVNULL,
        stdin=subprocess.DEVNULL,
        close_fds=True,
        env=my_env
    )


def _terminate_all_children_processes(args):
    pids = []

    for p in psutil.process_iter(["name", "cmdline"]):
        if "python" in p.info["name"]:
            cmdline = p.info["cmdline"]

            if cmdline is not None and len(cmdline) > 1 and "tools/py3/tinkoff-asap-trading/parallel.py" in cmdline[1] and args.account in cmdline:
                pids.append(p.pid)

    if len(pids) > 0:
        logger.info("Terminating children processes")

        for pid in pids:
            logger.info(f"Terminating child process with PID {pid}")
            os.kill(pid, signal.SIGTERM)


def _exit_gracefully(signum, frame):
    global terminated
    terminated = True


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

    if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
        logger.error("Please specify token with --token or --token-file")

        sys.exit(1)

    if args.account == "":
        logger.error("Please specify account ID with --account")

        sys.exit(1)

    if args.lose > 0:
        logger.error("Please specify valid lose value with --lose. Must be negative value (for example -3.0)")

        sys.exit(1)

    if args.yield_value <= 0:
        logger.error("Please specify valid yield value with --yield")

        sys.exit(1)

    if args.part < 0:
        logger.error("Please specify valid part value with --part")

        sys.exit(1)

    asyncio.run(asap_trading(args))
