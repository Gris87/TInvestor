import argparse
import csv
import json
import re
import requests
import sys
import time
import zipfile

from datetime import datetime, timezone
from http import HTTPStatus
from io import TextIOWrapper
from pathlib import Path


HISTORY_DATA_URL = "https://invest-public-api.tbank.ru/history-data"

HUNDRED_PERCENT = 100

MS_IN_SECOND = 1000
ONE_MINUTE   = 60 * MS_IN_SECOND
ONE_HOUR     = 60 * ONE_MINUTE
ONE_DAY      = 24 * ONE_HOUR

CSV_FIELD_FIGI = 0
CSV_FIELD_TIMESTAMP = 1
CSV_FIELD_OPEN_PRICE = 2
CSV_FIELD_CLOSE_PRICE = 3
CSV_FIELD_HIGH_PRICE = 4
CSV_FIELD_LOW_PRICE = 5
CSV_FIELD_VOLUME = 6

ZIP_FILENAME_REGEXP = re.compile(r".*_(\d{4})(\d{2})(\d{2})\.csv")


def process_stock(args):
    now = round(time.time() * MS_IN_SECOND)

    start_timestamp = now - args.day_range * ONE_DAY
    end_timestamp   = now

    data = _download_data(args, start_timestamp, end_timestamp)
    falls = _search_falls(args, data)

    res = {
        "ticker": args.ticker,
        "falls": falls,
    }

    print(json.dumps(res))

    return True


def _download_data(args, start_timestamp, end_timestamp):
    res = []

    cache_folder_path = Path(args.cache) / "huge_fall"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    start_datetime = datetime.fromtimestamp(start_timestamp / MS_IN_SECOND, timezone.utc)
    end_datetime = datetime.fromtimestamp(end_timestamp / MS_IN_SECOND, timezone.utc)

    start_year = start_datetime.year
    end_year = end_datetime.year

    for year in range(start_year, end_year + 1):
        zip_file_path = cache_folder_path / f"{args.instrument_id}_{year}.zip"

        if not zip_file_path.exists():
            params = {
                "instrumentId": args.instrument_id,
                "year": year
            }

            content = _download_file(params)

            if content is not None:
                with open(zip_file_path, "wb") as f:
                    f.write(content)

        if zip_file_path.exists():
            with zipfile.ZipFile(zip_file_path, "r") as z:
                for filename in sorted(z.namelist()):
                    match = ZIP_FILENAME_REGEXP.match(filename)

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


def _search_falls(args, data):
    res = []

    return res


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cache",
        dest="cache",
        type=str,
        default="build/Desktop-Debug/app/build/cache",
        help="Path to cache folder"
    )
    parser.add_argument(
        "--instrument-id",
        dest="instrument_id",
        type=str,
        default="",
        help="Instrument ID",
    )
    parser.add_argument(
        "--ticker",
        dest="ticker",
        type=str,
        default="",
        help="Instrument ticker",
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
    args = parser.parse_args()

    sys.exit(0 if process_stock(args) else 1)
