import argparse
import json
import sys

from pathlib import Path


BAD_YIELD = -3.0


def process_stock(args):
    preprocess_data = _load_preprocess_data(args)
    total_yield = _calculate_total_yield(args, preprocess_data)

    print(f"{args.spread};{args.min_yield};{total_yield:.1f}")

    return True


def _load_preprocess_data(args):
    res = {}

    with open(Path(args.cache) / "bidirinfo" / f"{args.instrument_id}.json", "r", encoding="utf-8") as f:
        content = f.read()
        res = json.loads(content)

    return res


def _calculate_total_yield(args, preprocess_data):
    total_yield = 0.0

    min_price_increment = preprocess_data["minPriceIncrement"]
    spreads = preprocess_data["spreads"]
    data = preprocess_data["data"]

    for spread in spreads:
        if spread["spread"] < args.spread:
            break

        if _is_good_to_buy(args, data, spread["index"], min_price_increment):
            total_yield += args.min_yield
        else:
            total_yield += BAD_YIELD

    return total_yield


def _is_good_to_buy(args, data, index, min_price_increment):
    return False


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
        "--spread",
        dest="spread",
        type=float,
        default=0.7,
        help="Keep spread between close price and next low price",
    )
    parser.add_argument(
        "--min-yield",
        dest="min_yield",
        type=float,
        default=0.1,
        help="Minimum yield",
    )
    args = parser.parse_args()

    sys.exit(0 if process_stock(args) else 1)
