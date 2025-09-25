import argparse
import json
import sys

import xlsxwriter
from datetime import datetime

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
    _generate_excel(args, stats)

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
    entries = []

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

            entries.append(entry)

    daily = {}

    for entry in entries:
        date = datetime.fromtimestamp(entry["timestamp"] / 1000.0).strftime('%Y-%m-%d')

        if date in daily:
            date_info = daily[date]
        else:
            date_info = {
                DECISION_1: 0,
                DECISION_2: 0,
                DECISION_3: 0,
                DECISION_4: 0,
                DECISION_HUGE_SPREAD: 0,
                DECISION_HIGH_LIQUIDITY: 0,
                DECISION_OTHER: 0,
                "Total": 0
            }

        yield_with_commission = entry["yieldWithCommission"]

        date_info[entry["decision"]] += yield_with_commission
        date_info["Total"] += yield_with_commission

        daily[date] = date_info

    combined = {
        DECISION_1: 0,
        DECISION_2: 0,
        DECISION_3: 0,
        DECISION_4: 0,
        DECISION_HUGE_SPREAD: 0,
        DECISION_HIGH_LIQUIDITY: 0,
        DECISION_OTHER: 0,
    }

    for entry in entries:
        combined[entry["decision"]] += entry["yieldWithCommission"]

    stats = {
        "entries": entries,
        "daily": daily,
        "combined": combined,
    }

    return stats


def _generate_excel(args, stats):
    workbook = xlsxwriter.Workbook(args.output)

    _create_stats_sheets(workbook, stats)
    _create_daily_decision_yield_sheets(workbook, stats)
    _create_combined_yield_sheets(workbook, stats)

    workbook.close()


def _create_stats_sheets(workbook, stats):
    worksheet = workbook.add_worksheet("Stats (Data)")

    worksheet.set_column("A:A", 17.57)
    worksheet.set_column("B:B", 37.43)
    worksheet.set_column("C:C", 7.86)
    worksheet.set_column("D:D", 62.14)
    worksheet.set_column("E:E", 81.57)
    worksheet.set_column("F:F", 12.14)
    worksheet.set_column("G:G", 23.0)

    data = []

    for entry in stats["entries"]:
        data.append(
            [
                datetime.fromtimestamp(entry["timestamp"] / 1000.0).strftime('%Y-%m-%d %H:%M:%S'),
                entry["instrumentId"],
                entry["instrumentTicker"],
                entry["instrumentName"],
                entry["description"],
                entry["decision"],
                entry["yieldWithCommission"],
            ]
        )

    worksheet.add_table(
        f"A1:G{len(data) + 1}",
        {
            "name": "AllStatsData",
            "style": "Table Style Medium 13",
            "data": data,
            "columns": [
                {"header": "Time"},
                {"header": "Instrument ID"},
                {"header": "Ticker"},
                {"header": "Name"},
                {"header": "Description"},
                {"header": "Decision"},
                {"header": "Yield with commission"},
            ],
        },
    )


def _create_daily_decision_yield_sheets(workbook, stats):
    worksheet = workbook.add_worksheet("Daily yield (Data)")

    worksheet.set_column("A:A", 17.57)
    worksheet.set_column("B:B", 17.57)
    worksheet.set_column("C:C", 17.57)
    worksheet.set_column("D:D", 17.57)
    worksheet.set_column("E:E", 17.57)
    worksheet.set_column("F:F", 17.57)
    worksheet.set_column("G:G", 17.57)
    worksheet.set_column("H:H", 17.57)
    worksheet.set_column("I:I", 17.57)

    data = []

    for date, entry in stats["daily"].items():
        data.append(
            [
                date,
                entry[DECISION_1],
                entry[DECISION_2],
                entry[DECISION_3],
                entry[DECISION_4],
                entry[DECISION_HUGE_SPREAD],
                entry[DECISION_HIGH_LIQUIDITY],
                entry[DECISION_OTHER],
                entry["Total"],
            ]
        )

    worksheet.add_table(
        f"A1:I{len(data) + 1}",
        {
            "name": "DailyDecisionYieldData",
            "style": "Table Style Medium 13",
            "data": data,
            "columns": [
                {"header": "Date"},
                {"header": DECISION_1},
                {"header": DECISION_2},
                {"header": DECISION_3},
                {"header": DECISION_4},
                {"header": DECISION_HUGE_SPREAD},
                {"header": DECISION_HIGH_LIQUIDITY},
                {"header": DECISION_OTHER},
                {"header": "Total"},
            ],
        },
    )


def _create_combined_yield_sheets(workbook, stats):
    worksheet = workbook.add_worksheet("Combined yield (Data)")

    worksheet.set_column("A:A", 17.57)
    worksheet.set_column("B:B", 17.57)
    worksheet.set_column("C:C", 17.57)
    worksheet.set_column("D:D", 17.57)
    worksheet.set_column("E:E", 17.57)
    worksheet.set_column("F:F", 17.57)
    worksheet.set_column("G:G", 17.57)

    combined = stats["combined"]
    data = [
        [
            combined[DECISION_1],
            combined[DECISION_2],
            combined[DECISION_3],
            combined[DECISION_4],
            combined[DECISION_HUGE_SPREAD],
            combined[DECISION_HIGH_LIQUIDITY],
            combined[DECISION_OTHER],
        ]
    ]

    worksheet.add_table(
        f"A1:G{len(data) + 1}",
        {
            "name": "CombinedYieldData",
            "style": "Table Style Medium 13",
            "data": data,
            "columns": [
                {"header": DECISION_1},
                {"header": DECISION_2},
                {"header": DECISION_3},
                {"header": DECISION_4},
                {"header": DECISION_HUGE_SPREAD},
                {"header": DECISION_HIGH_LIQUIDITY},
                {"header": DECISION_OTHER},
            ],
        },
    )


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
    parser.add_argument(
        "--output",
        dest="output",
        type=str,
        default="build/Desktop-Debug/operations_stats.xlsx",
        help="Output file"
    )
    args = parser.parse_args()

    sys.exit(0 if operations_stats(args) else 1)
