import argparse
import json
import os
import psutil
import requests
import struct
import subprocess
import sys
import time

from concurrent.futures import ThreadPoolExecutor
from loguru import logger
from pathlib import Path

from localization import *


PATH_TO_SCRIPT = Path(__file__).parent

TELEGRAM_TOKEN = os.environ["TELEGRAM_TOKEN"]
TELEGRAM_SEND_URL = f"https://api.telegram.org/bot{TELEGRAM_TOKEN}/sendMessage"

HUGE_SELL = 45.0
HUNDRED_PERCENT = 100.0
MS_IN_SECOND = 1000
ONE_MINUTE = 60 * MS_IN_SECOND
ONE_HOUR = 60 * ONE_MINUTE
ONE_DAY = 24 * ONE_HOUR
HUGE_SELL_INTERVAL = 5 * ONE_DAY
HUGE_SELL_STEP = 60


def telegram_bot(args):
    if args.dividends_only:
        return _check_dividends(args)

    _check_operations_json(args)
    _check_core_file(args)
    _check_app_running(args)

    if args.extra_huge_sell:
        _check_huge_sell(args)

    return True


def _check_dividends(args):
    with open(Path(args.path_to_stocks) / "stocks.json", "r", encoding="utf-8") as f:
        content = f.read()
        stocks_meta = json.loads(content)

    commands = []

    for stock_meta in stocks_meta:
        commands.append(
            [
                "python",
                str(Path(PATH_TO_SCRIPT) / "dividends_parallel.py"),
                "--chat-id", args.chat_id,
                "--ticker", stock_meta["instrumentTicker"]
            ]
        )

    return _execute_commands(commands)


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


def _check_operations_json(args):
    with open(args.path_to_operations, "r", encoding="utf-8") as f:
        content = f.read()
        operations = json.loads(f"[{content}]")

    now = round(time.time() * MS_IN_SECOND)
    last_timestamp = operations[-1]["timestamp"]

    delta = now - last_timestamp

    if delta > args.inactivity_days * ONE_DAY:
        _send_message(args, msg_operations_inactivity)


def _check_core_file(args):
    core_file = Path(args.path_to_operations).parent.parent.parent.parent / "core"

    if core_file.exists():
        _send_message(args, msg_core_file_found)

        now = round(time.time() * MS_IN_SECOND)
        os.rename(core_file, f"{core_file}_{now}")


def _check_app_running(args):
    found = False

    for proc in psutil.process_iter(['name']):
        if proc.info['name'] == "TInvestor":
            found = True

            break

    if not found:
        _send_message(args, msg_app_restart)

        home_directory = Path.home()
        subprocess.Popen(["xdg-open", f"{home_directory}/Desktop/TInvestor.desktop"], close_fds=True)


def _check_huge_sell(args):
    with open(Path(args.path_to_stocks) / "stocks.json", "r", encoding="utf-8") as f:
        content = f.read()
        stocks_meta = json.loads(content)

    # struct StockData
    # {
    #     qint64 timestamp;
    #     qint32 quantity;
    #     float  price;
    # };
    format_string = "qif"

    for stock_meta in stocks_meta:
        instrument_id = stock_meta["instrumentId"]

        data = []

        with open(Path(args.path_to_stocks) / f"{instrument_id}.dat", "rb") as data_file:
            for timestamp, quantity, price in struct.iter_unpack(format_string, data_file.read()):
                data.append((timestamp, price))

        # for i in range(len(data)):
        #     if _is_huge_sell_found(data, i):
        #         print(msg_huge_sell.format(ticker=stock_meta["instrumentTicker"], name=stock_meta["instrumentName"]))
        #         print(data[i][0])

        #         break

        if len(data) > 0 and _is_huge_sell_found(data, len(data) - 1):
            _send_message(args, msg_huge_sell.format(ticker=stock_meta["instrumentTicker"], name=stock_meta["instrumentName"]))


def _is_huge_sell_found(data, index):
    timestamp, price = data[index]

    limit_timestamp = timestamp - HUGE_SELL_INTERVAL
    maximum_price = price / (1 - (HUGE_SELL / HUNDRED_PERCENT))

    i = index - 1

    while i >= 0:
        prev_timestamp, prev_price  = data[i]

        if prev_timestamp < limit_timestamp:
            break

        if prev_price >= maximum_price:
            return True

        i -= HUGE_SELL_STEP

    return False


def _send_message(args, msg):
    logger.info(f"Send message: {msg}")

    requests.post(TELEGRAM_SEND_URL, json={"chat_id": args.chat_id, "text": msg})


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--chat-id",
        dest="chat_id",
        type=str,
        default="",
        help="Telegram chat ID"
    )
    parser.add_argument(
        "--path-to-operations",
        dest="path_to_operations",
        type=str,
        default="",
        help="Path to operations.json file"
    )
    parser.add_argument(
        "--path-to-stocks",
        dest="path_to_stocks",
        type=str,
        default="",
        help="Path to stocks folder"
    )
    parser.add_argument(
        "--inactivity-days",
        dest="inactivity_days",
        type=int,
        default=3,
        help="How many days we can stay idle"
    )
    parser.add_argument(
        "--extra-huge-sell",
        dest="extra_huge_sell",
        default=False,
        action="store_true",
        help="Send notifications about huge selling",
    )
    parser.add_argument(
        "--dividends-only",
        dest="dividends_only",
        default=False,
        action="store_true",
        help="Send notifications about dividends only",
    )
    args = parser.parse_args()

    if args.chat_id == "":
        logger.error("Please specify Telegram chat ID with --chat-id")

        sys.exit(1)

    if not args.dividends_only and args.path_to_operations == "":
        logger.error("Please specify path to operations.json file with --path-to-operations")

        sys.exit(1)

    if (args.extra_huge_sell or args.dividends_only) and args.path_to_stocks == "":
        logger.error("Please specify path to stocks folder with --path-to-stocks")

        sys.exit(1)

    sys.exit(0 if telegram_bot(args) else 1)
