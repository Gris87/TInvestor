import argparse
import sys


def process_stock(args):
    print("0.4;0.1;10.0")

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
        "--min-price-increment",
        dest="min_price_increment",
        type=float,
        default=0.1,
        help="Minimum price increment",
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
    parser.add_argument(
        "--start-timestamp",
        dest="start_timestamp",
        type=int,
        default=0,
        help="Start timestamp",
    )
    parser.add_argument(
        "--end-timestamp",
        dest="end_timestamp",
        type=int,
        default=0,
        help="End timestamp",
    )
    args = parser.parse_args()

    sys.exit(0 if process_stock(args) else 1)
