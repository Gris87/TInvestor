import argparse
import json
import hashlib
import sys

from datetime import datetime
from pathlib import Path


TIMESTAMP_STEP = 75000


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
        "timestamp": last_timestamp,
        "instrumentId": "15dc2120-29d2-48b8-87c0-da1d95255f68",  # SPBE
        "description": "a" * 100,
        "price": 100.5,
        "fifoItems": [],
        "avgPriceFifo": 100.5,
        "avgPriceWavg": 100.5,
        "quantity": 1000,
        "remainedQuantity": 1000,
        "payment": 100000,
        "avgCostFifo": 100000,
        "costFifo": {"units": 100000, "nano": 0},
        "costWavg": {"units": 100000, "nano": 0},
        "commission": -5.15,
        "yield": 0,
        "yieldWithCommission": -5.15,
        "yieldWithCommissionPercent": -0.05,
        "inputMoney": {"units": 100000, "nano": 0},
        "maxInputMoney": {"units": 100000, "nano": 0},
        "totalYieldWithCommission": {"units": -5, "nano": -150000000},
        "totalYieldWithCommissionPercent": -0.03,
        "remainedMoney": {"units": 100, "nano": 0},
        "totalMoney": {"units": 100000, "nano": 0},
        "paymentPrecision": 2,
        "commissionPrecision": 2
    }

    block.append(operation)
    last_timestamp += TIMESTAMP_STEP

    operation = {
        "timestamp": last_timestamp,
        "instrumentId": "15dc2120-29d2-48b8-87c0-da1d95255f68",  # SPBE
        "description": "b" * 100,
        "price": 120.5,
        "fifoItems": [],
        "avgPriceFifo": 100.5,
        "avgPriceWavg": 100.5,
        "quantity": 1000,
        "remainedQuantity": 0,
        "payment": 100000,
        "avgCostFifo": 100000,
        "costFifo": {"units": 100000, "nano": 0},
        "costWavg": {"units": 100000, "nano": 0},
        "commission": -5.15,
        "yield": 1000,
        "yieldWithCommission": 1000,
        "yieldWithCommissionPercent": 5,
        "inputMoney": {"units": 100000, "nano": 0},
        "maxInputMoney": {"units": 100000, "nano": 0},
        "totalYieldWithCommission": {"units": 1000, "nano": 0},
        "totalYieldWithCommissionPercent": 5.99,
        "remainedMoney": {"units": 100000, "nano": 0},
        "totalMoney": {"units": 100000, "nano": 0},
        "paymentPrecision": 2,
        "commissionPrecision": 2
    }

    block.append(operation)
    last_timestamp += TIMESTAMP_STEP

    operation = {
        "timestamp": last_timestamp,
        "instrumentId": "48bd9002-43be-4528-abf4-dc8135ad4550",  # UGLD
        "description": "c" * 100,
        "price": 100.5,
        "fifoItems": [],
        "avgPriceFifo": 100.5,
        "avgPriceWavg": 100.5,
        "quantity": 1000,
        "remainedQuantity": 1000,
        "payment": 100000,
        "avgCostFifo": 100000,
        "costFifo": {"units": 100000, "nano": 0},
        "costWavg": {"units": 100000, "nano": 0},
        "commission": -5.15,
        "yield": 0,
        "yieldWithCommission": -5.15,
        "yieldWithCommissionPercent": -0.05,
        "inputMoney": {"units": 100000, "nano": 0},
        "maxInputMoney": {"units": 100000, "nano": 0},
        "totalYieldWithCommission": {"units": -5000, "nano": -150000000},
        "totalYieldWithCommissionPercent": -0.03,
        "remainedMoney": {"units": 100, "nano": 0},
        "totalMoney": {"units": 100000, "nano": 0},
        "paymentPrecision": 2,
        "commissionPrecision": 2
    }

    block.append(operation)
    last_timestamp += TIMESTAMP_STEP

    operation = {
        "timestamp": last_timestamp,
        "instrumentId": "48bd9002-43be-4528-abf4-dc8135ad4550",  # UGLD
        "description": "d" * 100,
        "price": 120.5,
        "fifoItems": [],
        "avgPriceFifo": 100.5,
        "avgPriceWavg": 100.5,
        "quantity": 1000,
        "remainedQuantity": 0,
        "payment": 100000,
        "avgCostFifo": 100000,
        "costFifo": {"units": 100000, "nano": 0},
        "costWavg": {"units": 100000, "nano": 0},
        "commission": -5.15,
        "yield": 1000,
        "yieldWithCommission": 1000,
        "yieldWithCommissionPercent": 5,
        "inputMoney": {"units": 100000, "nano": 0},
        "maxInputMoney": {"units": 100000, "nano": 0},
        "totalYieldWithCommission": {"units": -20000, "nano": 0},
        "totalYieldWithCommissionPercent": -9.12,
        "remainedMoney": {"units": 100000, "nano": 0},
        "totalMoney": {"units": 100000, "nano": 0},
        "paymentPrecision": 2,
        "commissionPrecision": 2
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
        default="d1843f24-0864-4666-8608-d5d16822fbae",
        help="Account ID",
    )
    parser.add_argument(
        "--count",
        dest="count",
        type=int,
        default=100000,
        help="Amount of operations",
    )
    args = parser.parse_args()

    sys.exit(0 if generate_operations(args) else 1)
