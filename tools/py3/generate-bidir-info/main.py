import argparse
import csv
import json
import math
import os
import re
import requests
import subprocess
import sys
import time
import zipfile

from concurrent.futures import ThreadPoolExecutor
from datetime import datetime, timezone
from http import HTTPStatus
from io import TextIOWrapper
from pathlib import Path


path_to_script = Path(__file__).parent

HISTORY_DATA_URL = "https://invest-public-api.tinkoff.ru/history-data"

HUNDRED_PERCENT = 100

MS_IN_SECOND = 1000
ONE_MINUTE   = 60 * MS_IN_SECOND
ONE_HOUR     = 60 * ONE_MINUTE
ONE_DAY      = 24 * ONE_HOUR
ONE_MONTH    = 31 * ONE_DAY

MINIMUM_STEP_DELTA = 2 * ONE_HOUR
MINIMUM_SPREAD = 0.4
MINIMUM_YIELD_VARIANTS = [0.1, 0.2, 0.3, 0.4]

CSV_FIELD_FIGI = 0
CSV_FIELD_TIMESTAMP = 1
CSV_FIELD_OPEN_PRICE = 2
CSV_FIELD_CLOSE_PRICE = 3
CSV_FIELD_HIGH_PRICE = 4
CSV_FIELD_LOW_PRICE = 5
CSV_FIELD_VOLUME = 6

zip_filename_regexp = re.compile(r".*_(\d{4})(\d{2})(\d{2})\.csv")


def generate_bidir_info(args):
    stocks = _get_stocks(args)
    _process_stocks(args, stocks)

    return True


def _get_stocks(args):
    res = []

    with open(Path(args.path_to_stocks) / "stocks.json", "r", encoding="utf-8") as f:
        content = f.read()
        res = json.loads(content)

    return res


def _process_stocks(args, stocks):
    res = []

    print("========================================================")
    print("N          Stock      Spread    Min Yield    Total yield")
    print("========================================================")

    for i, stock in enumerate(stocks):
        instrument_ticker = stock["instrumentTicker"]

        print(f"{i+1:3}/{len(stocks)}    {instrument_ticker:11}", end="", flush=True)
        stock_result = _process_stock(args, stock)

        spread = stock_result["spread"]
        min_yield = stock_result["minYield"]
        total_yield = stock_result["totalYield"]

        print(f"{spread:3}%      {min_yield:3}%       {total_yield:5}%")

        res.append(stock_result)

    return res


def _process_stock(args, stock):
    res = {}

    instrument_id = stock["instrumentId"]

    now = round(time.time() * MS_IN_SECOND)

    start_timestamp = now - args.month_range * ONE_MONTH
    end_timestamp   = now

    data = _download_data(args, instrument_id, start_timestamp, end_timestamp)
    preprocess_data = _preprocess_stock(stock, data)

    preprocess_file = Path(args.cache) / "bidirinfo" / f"{instrument_id}.json"

    with open(preprocess_file, "w", encoding="utf-8") as f:
        json.dump(preprocess_data, f, ensure_ascii=False)

    max_spread = preprocess_data["maxSpread"]

    commands = []
    spread = MINIMUM_SPREAD

    while spread <= max_spread:
        for min_yield in MINIMUM_YIELD_VARIANTS:
            commands.append(
                [
                    "python",
                    str(Path(path_to_script) / "parallel.py"),
                    "--cache", args.cache,
                    "--instrument-id", instrument_id,
                    "--spread", f"{spread:.1f}",
                    "--min-yield", f"{min_yield:.1f}"
                ]
            )

        spread += 0.1

    res["spread"] = max_spread
    res["minYield"] = 0.3
    res["totalYield"] = 0.0

    success, output = _execute_commands(commands)

    if success:
        for line in output:
            fields = line.split(";")

            total_yield = float(fields[2])

            if total_yield > res["totalYield"]:
                res["spread"] = float(fields[0])
                res["minYield"] = float(fields[1])
                res["totalYield"] = total_yield

    return res


def _download_data(args, instrument_id, start_timestamp, end_timestamp):
    res = []

    cache_folder_path = Path(args.cache) / "bidirinfo"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    start_datetime = datetime.fromtimestamp(start_timestamp / MS_IN_SECOND, timezone.utc)
    end_datetime = datetime.fromtimestamp(end_timestamp / MS_IN_SECOND, timezone.utc)

    start_year = start_datetime.year
    end_year = end_datetime.year

    for year in range(start_year, end_year + 1):
        zip_file_path = cache_folder_path / f"{instrument_id}_{year}.zip"

        if not zip_file_path.exists():
            params = {
                "instrumentId": instrument_id,
                "year": year
            }

            content = _download_file(params)

            if content is not None:
                with open(zip_file_path, "wb") as f:
                    f.write(content)

        if zip_file_path.exists():
            with zipfile.ZipFile(zip_file_path, "r") as z:
                for filename in sorted(z.namelist()):
                    match = zip_filename_regexp.match(filename)

                    if match is not None:
                        data_year = int(match.group(1))
                        data_month = int(match.group(2))
                        data_day = int(match.group(3))

                        data_datetime = datetime(data_year, data_month, data_day, tzinfo=timezone.utc)

                        if data_datetime >= start_datetime and data_datetime < end_datetime:
                            with z.open(filename, "r") as f:
                                csv_reader = csv.reader(TextIOWrapper(f, "utf-8"), delimiter=";")

                                for row in csv_reader:
                                    entry = {
                                        "timestamp": int(datetime.fromisoformat(row[CSV_FIELD_TIMESTAMP]).timestamp() * 1000),
                                        "openPrice": float(row[CSV_FIELD_OPEN_PRICE]),
                                        "closePrice": float(row[CSV_FIELD_CLOSE_PRICE]),
                                        "highPrice": float(row[CSV_FIELD_HIGH_PRICE]),
                                        "lowPrice": float(row[CSV_FIELD_LOW_PRICE]),
                                    }

                                    res.append(entry)

    return res


def _download_file(params):
    while True:
        response = requests.get(HISTORY_DATA_URL, params=params)

        if response.status_code == HTTPStatus.TOO_MANY_REQUESTS:
            time.sleep(1)

            continue

        if response.status_code == HTTPStatus.OK:
            return response.content

        break

    return None


def _preprocess_stock(stock, data):
    spreads = []
    max_spread = 0.0

    for i in range(len(data) - 1):
        cur = data[i]
        next = data[i + 1]

        if next["timestamp"] - cur["timestamp"] < MINIMUM_STEP_DELTA:
            cur_close_price = cur["closePrice"]
            next_low_price = next["lowPrice"]

            spread = HUNDRED_PERCENT - (next_low_price / cur_close_price) * HUNDRED_PERCENT

            if spread >= MINIMUM_SPREAD:
                spreads.append({
                    "index": i,
                    "spread": spread
                })

            max_spread = max(max_spread, spread)

    spreads.sort(key=lambda x: x["spread"], reverse=True)

    return {
        "stock": stock,
        "spreads": spreads,
        "maxSpread": math.floor(max_spread * 10.0) / 10.0,
        "data": data
    }


def _execute_commands(commands):
    res = True
    output = []

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, lines in executor.map(_execute_command, commands):
            res &= result

            # for line in lines:
            #     print(line)

            output.append(lines[-1])

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

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return process.returncode == 0, lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--month-range",
        dest="month_range",
        type=int,
        default=1,
        help="Amount of months to check"
    )
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
        "--output",
        dest="output",
        type=str,
        default="app/assets/bidir_info.json",
        help="Output file"
    )
    args = parser.parse_args()

    sys.exit(0 if generate_bidir_info(args) else 1)
