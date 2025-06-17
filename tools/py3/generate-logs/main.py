import argparse
import json
import hashlib
import sys

from datetime import datetime
from pathlib import Path


TIMESTAMP_STEP = 75000


def generate_logs(args):
    account_hash = _calculate_account_hash(args.account)

    output_path = Path(f"build/Desktop-Debug/app/build/data/autopilot/{account_hash}/logs.json")
    output_path.parent.mkdir(parents=True, exist_ok=True)

    f = open(output_path, "w")
    f.close()

    last_timestamp = int(datetime.now().timestamp() * 1000 - args.count * TIMESTAMP_STEP)

    amount = 0

    while amount < args.count:
        block_size, last_timestamp = _generate_logs(last_timestamp, output_path)

        amount += block_size

    return True


def _generate_logs(last_timestamp, output_path):
    block = []

    log = {
        "timestamp": last_timestamp,
        "level": 0,  # VERBOSE
        "instrumentId": "15dc2120-29d2-48b8-87c0-da1d95255f68",  # SPBE
        "instrumentTicker": "SPBE",
        "instrumentName": "SPBEmushka",
        "message": "a" * 100
    }

    block.append(log)
    last_timestamp += TIMESTAMP_STEP

    log = {
        "timestamp": last_timestamp,
        "level": 1,  # DEBUG
        "instrumentId": "48bd9002-43be-4528-abf4-dc8135ad4550",  # UGLD
        "instrumentTicker": "UGLD",
        "instrumentName": "UGLDushka",
        "message": "b" * 100
    }

    block.append(log)
    last_timestamp += TIMESTAMP_STEP

    log = {
        "timestamp": last_timestamp,
        "level": 2,  # INFO
        "instrumentId": "4d813ab1-8bc9-4670-89ea-12bfbab6017d",  # SMLT
        "instrumentTicker": "SMLT",
        "instrumentName": "SMLTushka",
        "message": "c" * 100
    }

    block.append(log)
    last_timestamp += TIMESTAMP_STEP

    log = {
        "timestamp": last_timestamp,
        "level": 3,  # WARNING
        "instrumentId": "962e2a95-02a9-4171-abd7-aa198dbe643a",  # GAZP
        "instrumentTicker": "GAZP",
        "instrumentName": "GAZPromushka",
        "message": "d" * 100
    }

    block.append(log)
    last_timestamp += TIMESTAMP_STEP

    log = {
        "timestamp": last_timestamp,
        "level": 4,  # ERROR
        "instrumentId": "e6123145-9665-43e0-8413-cd61b8aa9b13",  # SBER
        "instrumentTicker": "SBER",
        "instrumentName": "SBERushka",
        "message": "e" * 100
    }

    block.append(log)
    last_timestamp += TIMESTAMP_STEP

    first_record = output_path.stat().st_size == 0

    with open(output_path, "a", newline="") as f:
        for log in block:
            if not first_record:
                f.write(",\n")
            else:
                first_record = False

            f.write(json.dumps(log))

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
        default="d1843f24-0864-4666-8608-d5d16822fbae",
        help="Account ID",
    )
    parser.add_argument(
        "--count",
        dest="count",
        type=int,
        default=1000000,
        help="Amount of logs",
    )
    args = parser.parse_args()

    sys.exit(0 if generate_logs(args) else 1)
