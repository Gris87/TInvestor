import argparse
import json
import hashlib
import sys

from datetime import datetime
from pathlib import Path


TIMESTAMP_STEP = 5000


def generate_operations(args):
    account_hash = _calculate_account_hash(args.account)

    output_path = Path(f"build/Desktop-Debug/app/build/data/autopilot/{account_hash}/operations.json")
    output_path.parent.mkdir(parents=True, exist_ok=True)

    f = open(output_path, "w")
    f.close()

    last_timestamp = int(datetime.now().timestamp() * 1000 - args.count * TIMESTAMP_STEP)

    amount = 0

    while amount < args.count:
        block_size, last_timestamp = _generate_operations(last_timestamp, output_path)

        amount += block_size

    return True


def _generate_operations(last_timestamp, output_path):
    block = []

    operation = {
        "timestamp": last_timestamp
    }

    block.append(operation)
    last_timestamp += TIMESTAMP_STEP

    first_record = output_path.stat().st_size == 0

    with open(output_path, "a") as f:
        for operation in block:
            if not first_record:
                f.write(",\n")
            else:
                first_record = False

            f.write(json.dumps(operation))

    return len(block), last_timestamp


def _calculate_account_hash(account_id):
    md5_hash = hashlib.md5()
    md5_hash.update(account_id.encode("utf-8"))
    return md5_hash.hexdigest()


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--account",
        dest="account",
        type=str,
        default="d1843f24-0864-4666-8608-d5d16822fbae :)",
        help="Account ID",
    )
    parser.add_argument(
        "--count",
        dest="count",
        type=str,
        default=10,
        help="Amount of operations",
    )
    args = parser.parse_args()

    sys.exit(0 if generate_operations(args) else 1)
