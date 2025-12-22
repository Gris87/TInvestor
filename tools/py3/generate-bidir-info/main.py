import argparse
import json
import requests
import sys
import time

from datetime import datetime, timezone
from http import HTTPStatus
from pathlib import Path


HISTORY_DATA_URL = "https://invest-public-api.tinkoff.ru/history-data"

MS_IN_SECOND = 1000
ONE_MINUTE   = 60 * MS_IN_SECOND
ONE_HOUR     = 60 * ONE_MINUTE
ONE_DAY      = 24 * ONE_HOUR
ONE_MONTH    = 31 * ONE_DAY


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

    print("=========================================================")
    print("N        Stock    Spread  Min Yield  Total yield")
    print("=========================================================")

    for i, stock in enumerate(stocks):
        instrument_ticker = stock["instrumentTicker"]

        print(f"{i+1:3}/{len(stocks)}  {instrument_ticker:9}", end="", flush=True)
        stock_result, total_yield = _process_stock(args, stock)

        spread = stock_result["spread"]
        min_yield = stock_result["minYield"]

        print(f"{spread:3}%    {min_yield:3}%     {total_yield:5}%")

        res.append(stock_result)

    return res


def _process_stock(args, stock):
    res = {}
    total_yield = 0.0

    now = round(time.time() * MS_IN_SECOND)

    start_timestamp = now - args.month_range * ONE_MONTH
    end_timestamp   = now

    _download_data(args, stock, start_timestamp, end_timestamp)

    res["spread"] = 0.7
    res["minYield"] = 0.3

    return res, total_yield


def _download_data(args, stock, start_timestamp, end_timestamp):
    instrument_id = stock["instrumentId"]

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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--month-range",
        dest="month_range",
        type=int,
        default=21,  # TODO: 1
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
