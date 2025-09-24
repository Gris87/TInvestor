import argparse
import json
import sys

from localization import *


LOG_LEVEL_DEBUG = 1

DECISION_1 = "Decision 1"
DECISION_2 = "Decision 2"
DECISION_3 = "Decision 3"
DECISION_4 = "Decision 4"
DECISION_HUGE_SPREAD = "Huge spread"
DECISION_HIGH_LIQUIDITY = "High liquidity"
DECISION_OTHER = "Other"


def operations_stats(args):
    operations = _read_operations(args)
    logs = _read_logs(args)

    stats = _collect_statistics(operations, logs)

    return True


def _read_operations(args):
    with open(args.path_to_operations, "r", encoding="utf-8") as f:
        content = f.read()
        operations = json.loads(f"[{content}]")

    return operations


def _read_logs(args):
    with open(args.path_to_logs, "r", encoding="utf-8") as f:
        content = f.read()
        logs = json.loads(f"[{content}]")

    return logs


def _collect_statistics(operations, logs):
    stats = []

    for operation in operations:
        operation_description = operation["description"]

        if operation_description.startswith(text_sale):
            operation_timestamp = operation["timestamp"]
            operation_instrumentId = operation["instrumentId"]

            last_log_message = None

            for log in logs:
                log_timestamp = log["timestamp"]

                if log_timestamp > operation_timestamp:
                    break

                log_instrumentId = log["instrumentId"]
                log_message = log["message"]
                log_level = log["level"]

                if operation_instrumentId == log_instrumentId and log_level == LOG_LEVEL_DEBUG and not log_message.startswith(text_trade_interrupted) and not log_message.startswith(text_want_to_sell):
                    last_log_message = log_message

            entry = {
                "timestamp": operation_timestamp,
                "instrumentId": operation_instrumentId,
                "instrumentTicker": operation["instrumentTicker"],
                "instrumentName": operation["instrumentName"],
                "description": operation_description,
                "decision": _get_buy_decision_from_log_message(last_log_message),
                "yieldWithCommission": operation["yieldWithCommission"],
            }

            stats.append(entry)

    return stats


def _get_buy_decision_from_log_message(log_message):
    if log_message is None:
        return DECISION_OTHER

    match = decision_1_regexp.match(log_message)

    if match is not None:
        return DECISION_1

    match = decision_2_regexp.match(log_message)

    if match is not None:
        return DECISION_2

    match = decision_3_regexp.match(log_message)

    if match is not None:
        return DECISION_3

    match = decision_4_regexp.match(log_message)

    if match is not None:
        return DECISION_4

    match = huge_spread_regexp.match(log_message)

    if match is not None:
        return DECISION_HUGE_SPREAD

    match = high_liquidity_regexp.match(log_message)

    if match is not None:
        return DECISION_HIGH_LIQUIDITY

    return DECISION_OTHER


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--path-to-operations",
        dest="path_to_operations",
        type=str,
        default="build/Desktop-Debug/app/build/data/autopilot/4fa4713d40ede79c630b609d3a95c5ed/operations.json",
        help="Path to operations.json file"
    )
    parser.add_argument(
        "--path-to-logs",
        dest="path_to_logs",
        type=str,
        default="build/Desktop-Debug/app/build/data/autopilot/4fa4713d40ede79c630b609d3a95c5ed/logs.json",
        help="Path to logs.json file"
    )
    args = parser.parse_args()

    sys.exit(0 if operations_stats(args) else 1)
