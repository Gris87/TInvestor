import argparse
import json
import math
import sys

from pathlib import Path


HUNDRED_PERCENT = 100

MS_IN_SECOND = 1000
ONE_MINUTE   = 60 * MS_IN_SECOND
ONE_HOUR     = 60 * ONE_MINUTE

MAXIMUM_CHECK_DELTA = 1 * ONE_HOUR

GOOD_COUNT_DOUBLE_CHECK = 3
BAD_YIELD = -2.0
COMMISSION = 0.04


def process_stock(args):
    preprocess_data = _load_preprocess_data(args)
    total_yield = _calculate_total_yield(args, preprocess_data)

    print(f"{args.spread};{args.min_yield};{total_yield:.1f}")

    return True


def _load_preprocess_data(args):
    res = {}

    with open(Path(args.cache) / "bidirinfo" / f"{args.instrument_id}.json", "r", encoding="utf-8") as f:
        res = json.loads(f.read())

    return res


def _calculate_total_yield(args, preprocess_data):
    total_yield = 0.0

    min_price_increment = preprocess_data["minPriceIncrement"]
    spreads = preprocess_data["spreads"]
    data = preprocess_data["data"]

    index = 0

    for spread in spreads:
        if spread["index"] < index:
            continue

        if spread["spread"] < args.spread:
            continue

        ok, index = _is_good_to_buy(args, data, spread["index"], min_price_increment)

        if ok:
            total_yield += args.min_yield
        else:
            total_yield += BAD_YIELD

    return total_yield


def _is_good_to_buy(args, data, index, min_price_increment):
    cur = data[index]
    next = data[index + 1]

    start_timestamp = cur["timestamp"]

    cur_close_price = cur["closePrice"]
    buy_price = cur_close_price * (1 - (args.spread / HUNDRED_PERCENT))
    buy_price = max(math.floor(buy_price / min_price_increment) * min_price_increment, next["lowPrice"])

    good_sell_price = buy_price * (1 + ((args.min_yield + 2 * COMMISSION) / HUNDRED_PERCENT))
    good_sell_price = (math.ceil(good_sell_price / min_price_increment) + 1) * min_price_increment
    bad_sell_price = buy_price * (1 + ((BAD_YIELD + 2 * COMMISSION) / HUNDRED_PERCENT))

    index += 1
    good_count = 0

    while index < len(data):
        cur = data[index]

        if cur["timestamp"] - start_timestamp >= MAXIMUM_CHECK_DELTA:
            break

        if cur["closePrice"] <= bad_sell_price:
            break

        if cur["highPrice"] >= good_sell_price:
            good_count += 1

            if good_count >= GOOD_COUNT_DOUBLE_CHECK:
                return True, index

        index += 1

    return False, index


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
        default=1.0,
        help="Keep spread between close price and next low price",
    )
    parser.add_argument(
        "--min-yield",
        dest="min_yield",
        type=float,
        default=0.7,
        help="Minimum yield",
    )
    args = parser.parse_args()

    sys.exit(0 if process_stock(args) else 1)
