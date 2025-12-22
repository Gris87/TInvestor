import argparse
import json
import sys

from pathlib import Path


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

        print(f"{i+1:3}/{len(stocks)}  {instrument_ticker:7}", end="")
        stock_result, total_yield = _process_stock(args, stock)

        spread = stock_result["spread"]
        min_yield = stock_result["minYield"]

        print(f"{spread:5}%  {min_yield:5}%     {total_yield:5}%")

        res.append(stock_result)

    return res


def _process_stock(args, stock):
    res = {}
    total_yield = 0.0

    res["spread"] = 0.7
    res["minYield"] = 0.3

    return res, total_yield


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
        "--output",
        dest="output",
        type=str,
        default="app/assets/bidir_info.json",
        help="Output file"
    )
    args = parser.parse_args()

    sys.exit(0 if generate_bidir_info(args) else 1)
