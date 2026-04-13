import json
import os
import subprocess
import time

from concurrent.futures import ThreadPoolExecutor
from decimal import Decimal
from loguru import logger
from pathlib import Path

from localization import *
from messaging import send_message
from tinkoff.invest import Client, GetOperationsByCursorRequest
from tinkoff.invest.constants import INVEST_GRPC_API
from tinkoff.invest.schemas import OperationState
from tinkoff.invest.utils import quotation_to_decimal


PATH_TO_SCRIPT = Path(__file__).parent


def check_portfolio(args):
    commands = []
    positions = {}

    cache_folder_path = Path(args.cache) / "positions"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    positions_path = cache_folder_path / "positions.json"

    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)

    with Client(token, target=INVEST_GRPC_API) as client:
        logger.info("Verifying account")

        if not _validate_account(client, args.account):
            return

        logger.info("Get portfolio")

        portfolio = _get_portfolio(client, args.account)

        for position in portfolio.positions:
            positions[position.instrument_uid] = position.quantity.__dict__

            if position.instrument_type=='currency':
                continue

            commands.append(
                [
                    "python",
                    str(Path(PATH_TO_SCRIPT) / "pulse_parallel.py"),
                    "--ticker", position.ticker
                ]
            )

    if positions_path.exists():
        with open(positions_path, "r", encoding="utf-8") as f:
            old_positions = json.loads(f.read())

        if positions != old_positions:
            send_message(msg_positions_changed + "\n\n" + _describe_portfolio(portfolio.positions))

    with open(positions_path, "w", encoding="utf-8") as f:
        json.dump(positions, f, ensure_ascii=False)

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


def _describe_portfolio(positions):
    total_cost = Decimal("0.10")
    currency_info = ""
    stocks_info = ""

    for position in positions:
        if position.instrument_type=='currency':
            currency_info += "\n" + msg_currency_cost.format(currency=position.current_price.currency.upper(), cost=quotation_to_decimal(position.quantity))
            total_cost += quotation_to_decimal(position.quantity)
        else:
            cost = quotation_to_decimal(position.quantity) * quotation_to_decimal(position.average_position_price_fifo)

            stocks_info += "\n" + msg_stock_cost.format(ticker=position.ticker, quantity=quotation_to_decimal(position.quantity), cost=cost)
            total_cost += cost

    cost_info = msg_total_cost.format(total_cost=total_cost)

    return cost_info + "\n------------------------------------" + currency_info + stocks_info


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
