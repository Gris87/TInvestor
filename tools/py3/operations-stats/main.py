import argparse
import json
import sys

from localization import *


LOG_LEVEL_DEBUG = 1


def operations_stats(args):
    operations = _read_operations(args)
    logs = _read_logs(args)

    stats = _collect_statistics(operations, logs)
    print(stats)

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
            operation_yield = operation["yield"]

            last_log = None

            for log in logs:
                log_timestamp = log["timestamp"]

                if log_timestamp > operation_timestamp:
                    break

                log_instrumentId = log["instrumentId"]
                log_message = log["message"]
                log_level = log["level"]

                if operation_instrumentId == log_instrumentId and log_level == LOG_LEVEL_DEBUG and not log_message.startswith(text_want_to_sell):
                    last_log = log

            print(last_log)

    return stats


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
