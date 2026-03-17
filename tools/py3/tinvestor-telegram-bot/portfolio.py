import json
import os
import subprocess
import time

from concurrent.futures import ThreadPoolExecutor
from loguru import logger
from pathlib import Path

from tinkoff.invest import Client, GetOperationsByCursorRequest
from tinkoff.invest.constants import INVEST_GRPC_API
from tinkoff.invest.schemas import OperationState


PATH_TO_SCRIPT = Path(__file__).parent

RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c"


def check_portfolio(args):
    commands = []

    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)

    with Client(token, target=INVEST_GRPC_API) as client:
        logger.info("Verifying account")

        if not _validate_account(client, args.account):
            return

        logger.info("Get portfolio")

        portfolio = _get_portfolio(client, args.account)

        for position in portfolio.positions:
            if position.instrument_type=='currency':
                continue

            commands.append(
                [
                    "python",
                    str(Path(PATH_TO_SCRIPT) / "pulse_parallel.py"),
                    "--ticker", position.ticker
                ]
            )

    return _execute_commands(commands)


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


def _validate_account(client, account_id):
    accounts = {}

    tinkoff_accounts = client.users.get_accounts()

    for account in tinkoff_accounts.accounts:
        accounts[account.id] = account.name

    if account_id not in accounts:
        logger.error(f'Account "{account_id}" not found. Please use one of the following')

        for id, name in accounts.items():
            print(f"{id:36} | {name}")

        return False

    return True


def _get_portfolio(client, account_id):
    return client.operations.get_portfolio(account_id=account_id)


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, lines in executor.map(_execute_command, commands):
            res &= result

            for line in lines:
                print(line)

    return res


def _execute_command(command):
    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )

    lines = []

    encoding = os.device_encoding(1)
    if encoding is None:
        encoding = "utf-8"

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return process.returncode == 0, lines
