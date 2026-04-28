import argparse
import json
import os
import psutil
import struct
import subprocess
import sys
import time

from loguru import logger
from pathlib import Path

from dividends import check_dividends
from localization import *
from messaging import store_message
from portfolio import check_portfolio
from pulse import check_pulse


HUGE_SELL          = 45.0
HUNDRED_PERCENT    = 100.0
MS_IN_SECOND       = 1000
ONE_MINUTE         = 60 * MS_IN_SECOND
ONE_HOUR           = 60 * ONE_MINUTE
ONE_DAY            = 24 * ONE_HOUR
HUGE_SELL_INTERVAL = 5 * ONE_DAY
HUGE_SELL_STEP     = 60


def notifier(args):
    if args.dividends_only:
        return check_dividends(args)

    if args.pulse_only:
        return check_pulse(args)

    if args.portfolio_only:
        return check_portfolio(args)

    _check_operations_json(args)
    _check_core_file(args)
    _check_app_running(args)

    if args.extra_huge_sell:
        _check_huge_sell(args)

    return True


def _check_operations_json(args):
    with open(args.path_to_operations, "r", encoding="utf-8") as f:
        content = f.read()
        operations = json.loads(f"[{content}]")

    now = round(time.time() * MS_IN_SECOND)
    last_timestamp = operations[-1]["timestamp"]

    delta = now - last_timestamp

    if delta > args.inactivity_days * ONE_DAY:
        store_message(args, "system", msg_operations_inactivity)


def _check_core_file(args):
    core_file = Path(args.path_to_operations).parent.parent.parent.parent / "core"

    if core_file.exists():
        store_message(args, "system", msg_core_file_found)

        now = round(time.time() * MS_IN_SECOND)
        os.rename(core_file, f"{core_file}_{now}")


def _check_app_running(args):
    found = False

    for proc in psutil.process_iter(["name"]):
        if proc.info["name"] == "TInvestor":
            found = True

            break

    if not found:
        store_message(args, "system", msg_app_restart)

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
            store_message(args, "huge_sell", msg_recommend_to_buy + "\n" + msg_huge_sell.format(ticker=stock_meta["instrumentTicker"], name=stock_meta["instrumentName"]))


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
    parser.add_argument(
        "--account",
        dest="account",
        type=str,
        default="",
        help="Account ID",
    )
    parser.add_argument(
        "--cache",
        dest="cache",
        type=str,
        default="",
        help="Path to cache folder"
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
    parser.add_argument(
        "--pulse-only",
        dest="pulse_only",
        default=False,
        action="store_true",
        help="Send notifications about interesting pulse posts",
    )
    parser.add_argument(
        "--portfolio-only",
        dest="portfolio_only",
        default=False,
        action="store_true",
        help="Send notifications about interesting pulse posts for stocks in portfolio",
    )
    parser.add_argument(
        "--output",
        dest="output",
        type=str,
        default="",
        help="Output folder"
    )
    args = parser.parse_args()

    expected_env_vars = [
        "DISPLAY"
    ]
    good = True

    for var in expected_env_vars:
        if var not in os.environ:
            logger.error(f"Environment variable {var} is not set properly")

            good = False

    if not good:
        sys.exit(1)

    if args.cache == "":
        logger.error("Please specify path to cache folder with --cache")

        sys.exit(1)

    if not args.dividends_only and not args.pulse_only and not args.portfolio_only and args.path_to_operations == "":
        logger.error("Please specify path to operations.json file with --path-to-operations")

        sys.exit(1)

    if (args.extra_huge_sell or args.pulse_only) and args.path_to_stocks == "":
        logger.error("Please specify path to stocks folder with --path-to-stocks")

        sys.exit(1)

    if args.portfolio_only:
        if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
            logger.error("Please specify token with --token or --token-file")

            sys.exit(1)

        if args.account == "":
            logger.error("Please specify account ID with --account")

            sys.exit(1)

    if args.output == "":
        logger.error("Please specify path to output folder with --output")

        sys.exit(1)

    sys.exit(0 if notifier(args) else 1)
