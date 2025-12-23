import csv
import re
import zipfile

from datetime import datetime, timezone
from io import TextIOWrapper
from pathlib import Path


MS_IN_SECOND = 1000

CSV_FIELD_FIGI = 0
CSV_FIELD_TIMESTAMP = 1
CSV_FIELD_OPEN_PRICE = 2
CSV_FIELD_CLOSE_PRICE = 3
CSV_FIELD_HIGH_PRICE = 4
CSV_FIELD_LOW_PRICE = 5
CSV_FIELD_VOLUME = 6

zip_filename_regexp = re.compile(r".*_(\d{4})(\d{2})(\d{2})\.csv")


def load_data(args, stock, start_timestamp, end_timestamp):
    res = []

    instrument_id = stock["instrumentId"]
    cache_folder_path = Path(args.cache) / "bidirinfo"

    start_datetime = datetime.fromtimestamp(start_timestamp / MS_IN_SECOND, timezone.utc)
    end_datetime = datetime.fromtimestamp(end_timestamp / MS_IN_SECOND, timezone.utc)

    start_year = start_datetime.year
    end_year = end_datetime.year

    for year in range(start_year, end_year + 1):
        zip_file_path = cache_folder_path / f"{instrument_id}_{year}.zip"

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
                                        "openPrice": row[CSV_FIELD_OPEN_PRICE],
                                        "closePrice": row[CSV_FIELD_CLOSE_PRICE],
                                        "highPrice": row[CSV_FIELD_HIGH_PRICE],
                                        "lowPrice": row[CSV_FIELD_LOW_PRICE],
                                    }

                                    res.append(entry)

    return res
