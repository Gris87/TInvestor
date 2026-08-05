import argparse
import json
import sys

from pathlib import Path


def process_stock(args):
    cache_folder_path = Path(args.cache) / "huge_fall"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    falls = []

    falls.append({
        "timestamp": "2026-08-05 00:00:00",
        "ticker": args.ticker,
        "fall": -5.0,
    })

    res = {
        "ticker": args.ticker,
        "falls": falls,
    }

    print(json.dumps(res))

    return True


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
