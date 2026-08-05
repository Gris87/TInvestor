import argparse
import json
import os
import shutil
import subprocess
import sys

from concurrent.futures import ThreadPoolExecutor
from loguru import logger
from pathlib import Path


PATH_TO_SCRIPT = Path(__file__).parent


def search_huge_fall(args):
    stocks = _get_stocks(args)
    _process_stocks(args, stocks)

    shutil.rmtree(Path(args.cache) / "huge_fall")

    return True


def _get_stocks(args):
    res = []

    with open(Path(args.path_to_stocks) / "stocks.json", "r", encoding="utf-8") as f:
        res = json.loads(f.read())

    res.sort(key=lambda x: x["instrumentTicker"])

    return res


def _process_stocks(args, stocks):
    commands = []

    for stock in stocks:
        commands.append(
            [
                "python",
                str(Path(PATH_TO_SCRIPT) / "parallel.py"),
                "--cache", args.cache,
                "--instrument-id", stock["instrumentId"],
                "--ticker", stock["instrumentTicker"],
                "--day-range", f"{args.day_range}",
                "--fall", f"{args.fall:.1f}",
            ]
        )

    success, output = _execute_commands(commands)

    if success:
        output.sort(key=lambda x: x[args.sort_by])

        print("========================================")
        print("Time                   Stock      Fall")
        print("========================================")

        for stock_result in output:
            timestamp = stock_result["timestamp"]
            ticker = stock_result["ticker"]
            fall = stock_result["fall"]

            print(f"{timestamp:19}    {ticker:7}    {fall:3}%")


def _execute_commands(commands):
    res = True
    output = []

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, lines in executor.map(_execute_command, commands):
            res &= result

            # for line in lines:
            #     print(line)

            data = json.loads(lines[-1])
            logger.info("Data processed for {ticker}".format(ticker=data["ticker"]))

            output.extend(data["falls"])

    return res, output


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

    for line in iter(process.stdout.readline, b""):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return process.returncode == 0, lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--path-to-stocks",
        dest="path_to_stocks",
        type=str,
        default="build/Desktop-Debug/app/build/data/stocks",
        help="Path to stocks folder"
    )
    parser.add_argument(
        "--cache",
        dest="cache",
        type=str,
        default="build/Desktop-Debug/app/build/cache",
        help="Path to cache folder"
    )
    parser.add_argument(
        "--day-range",
        dest="day_range",
        type=int,
        default=14,
        help="Amount of days to check"
    )
    parser.add_argument(
        "--fall",
        dest="fall",
        type=float,
        default=3.0,
        help="Search for specified fall",
    )
    parser.add_argument(
        "--sort-by",
        dest="sort_by",
        type=str,
        choices=["timestamp", "ticker", "fall"],
        default="timestamp",
        help="Sort results by data type",
    )
    args = parser.parse_args()

    sys.exit(0 if search_huge_fall(args) else 1)
