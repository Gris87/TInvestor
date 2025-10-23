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
DECISION_5 = "Decision 5"
DECISION_6 = "Decision 6"
DECISION_7 = "Decision 7"
DECISION_8 = "Decision 8"
DECISION_HUGE_BID = "Huge bid"
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

            # if operation_timestamp < datetime(2025, 9, 25, 0, 0, 0).timestamp() * 1000:
            #     continue

            # if operation["instrumentTicker"] != "AAAA":
            #     continue

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
                DECISION_5: 0,
                DECISION_6: 0,
                DECISION_7: 0,
                DECISION_8: 0,
                DECISION_HUGE_BID: 0,
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
        DECISION_5: 0,
        DECISION_6: 0,
        DECISION_7: 0,
        DECISION_8: 0,
        DECISION_HUGE_BID: 0,
        DECISION_HUGE_SPREAD: 0,
        DECISION_HIGH_LIQUIDITY: 0,
        DECISION_OTHER: 0,
    }
    combined_positive = {
        DECISION_1: 0,
        DECISION_2: 0,
        DECISION_3: 0,
        DECISION_4: 0,
        DECISION_5: 0,
        DECISION_6: 0,
        DECISION_7: 0,
        DECISION_8: 0,
        DECISION_HUGE_BID: 0,
        DECISION_HUGE_SPREAD: 0,
        DECISION_HIGH_LIQUIDITY: 0,
        DECISION_OTHER: 0,
    }
    combined_negative = {
        DECISION_1: 0,
        DECISION_2: 0,
        DECISION_3: 0,
        DECISION_4: 0,
        DECISION_5: 0,
        DECISION_6: 0,
        DECISION_7: 0,
        DECISION_8: 0,
        DECISION_HUGE_BID: 0,
        DECISION_HUGE_SPREAD: 0,
        DECISION_HIGH_LIQUIDITY: 0,
        DECISION_OTHER: 0,
    }

    yield_per_decision = {}

    for entry in entries:
        instrument_name = entry["instrumentName"]
        decision = entry["decision"]
        yield_with_commission = entry["yieldWithCommission"]

        if yield_with_commission > 0:
            combined_positive[decision] += yield_with_commission
        else:
            combined_negative[decision] -= yield_with_commission

        combined[decision] += yield_with_commission

        if decision in yield_per_decision:
            decision_info = yield_per_decision[decision]
        else:
            decision_info = {}

        if instrument_name in decision_info:
            decision_info[instrument_name] += yield_with_commission
        else:
            decision_info[instrument_name] = yield_with_commission

        yield_per_decision[decision] = decision_info

    stats = {
        "entries": entries,
        "daily": daily,
        "combined": combined,
        "combined_positive": combined_positive,
        "combined_negative": combined_negative,
        "yield_per_decision": yield_per_decision,
    }

    return stats


def _generate_excel(args, stats):
    workbook = xlsxwriter.Workbook(args.output)

    _create_daily_decision_yield_chartsheet(workbook, stats)
    _create_combined_yield_chartsheet(workbook, stats)
    _create_combined_positive_yield_chartsheet(workbook, stats)
    _create_combined_negative_yield_chartsheet(workbook, stats)
    _create_yield_per_decision_chartsheet(workbook, stats)
    _create_stats_sheet(workbook, stats)
    _create_daily_decision_yield_sheet(workbook, stats)
    _create_combined_yield_sheet(workbook, stats)
    _create_combined_positive_yield_sheet(workbook, stats)
    _create_combined_negative_yield_sheet(workbook, stats)
    _create_yield_per_decision_sheets(workbook, stats)

    workbook.close()


def _create_stats_sheet(workbook, stats):
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


def _create_daily_decision_yield_sheet(workbook, stats):
    worksheet = workbook.add_worksheet("Daily yield (Data)")

    worksheet.set_column("A:A", 9.71)
    worksheet.set_column("B:B", 12.0)
    worksheet.set_column("C:C", 11.57)
    worksheet.set_column("D:D", 12.0)
    worksheet.set_column("E:E", 11.57)
    worksheet.set_column("F:F", 11.57)
    worksheet.set_column("G:G", 11.57)
    worksheet.set_column("H:H", 11.57)
    worksheet.set_column("I:I", 11.57)
    worksheet.set_column("J:J", 11.57)
    worksheet.set_column("K:K", 13.57)
    worksheet.set_column("L:L", 14.43)
    worksheet.set_column("M:M", 11.29)
    worksheet.set_column("N:N", 12.0)

    data = []

    for date, entry in stats["daily"].items():
        data.append(
            [
                date,
                entry[DECISION_1],
                entry[DECISION_2],
                entry[DECISION_3],
                entry[DECISION_4],
                entry[DECISION_5],
                entry[DECISION_6],
                entry[DECISION_7],
                entry[DECISION_8],
                entry[DECISION_HUGE_BID],
                entry[DECISION_HUGE_SPREAD],
                entry[DECISION_HIGH_LIQUIDITY],
                entry[DECISION_OTHER],
                entry["Total"],
            ]
        )

    worksheet.add_table(
        f"A1:N{len(data) + 1}",
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
                {"header": DECISION_5},
                {"header": DECISION_6},
                {"header": DECISION_7},
                {"header": DECISION_8},
                {"header": DECISION_HUGE_BID},
                {"header": DECISION_HUGE_SPREAD},
                {"header": DECISION_HIGH_LIQUIDITY},
                {"header": DECISION_OTHER},
                {"header": "Total"},
            ],
        },
    )


def _create_daily_decision_yield_chartsheet(workbook, stats):
    chartsheet = workbook.add_chartsheet("Daily yield")
    chart = workbook.add_chart({"type": "column"})

    chart.set_title({"name": "Daily yield"})

    last_row = len(stats["daily"]) + 1

    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!B1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!B2:B{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!C1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!C2:C{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!D1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!D2:D{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!E1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!E2:E{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!F1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!F2:F{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!G1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!G2:G{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!H1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!H2:H{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!I1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!I2:I{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!J1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!J2:J{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!K1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!K2:K{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!L1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!L2:L{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!M1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!M2:M{last_row}",
        }
    )
    chart.add_series(
        {
            "name": f"='Daily yield (Data)'!N1",
            "categories": f"='Daily yield (Data)'!A2:A{last_row}",
            "values": f"='Daily yield (Data)'!N2:N{last_row}",
        }
    )

    chartsheet.set_chart(chart)


def _create_combined_yield_sheet(workbook, stats):
    worksheet = workbook.add_worksheet("Combined yield (Data)")

    worksheet.set_column("A:A", 12.0)
    worksheet.set_column("B:B", 11.57)
    worksheet.set_column("C:C", 11.57)
    worksheet.set_column("D:D", 11.57)
    worksheet.set_column("E:E", 11.57)
    worksheet.set_column("F:F", 11.57)
    worksheet.set_column("G:G", 11.57)
    worksheet.set_column("H:H", 11.57)
    worksheet.set_column("I:I", 11.57)
    worksheet.set_column("J:J", 13.57)
    worksheet.set_column("K:K", 14.43)
    worksheet.set_column("L:L", 11.29)

    combined = stats["combined"]
    data = [
        [
            combined[DECISION_1],
            combined[DECISION_2],
            combined[DECISION_3],
            combined[DECISION_4],
            combined[DECISION_5],
            combined[DECISION_6],
            combined[DECISION_7],
            combined[DECISION_8],
            combined[DECISION_HUGE_BID],
            combined[DECISION_HUGE_SPREAD],
            combined[DECISION_HIGH_LIQUIDITY],
            combined[DECISION_OTHER],
        ]
    ]

    worksheet.add_table(
        f"A1:L{len(data) + 1}",
        {
            "name": "CombinedYieldData",
            "style": "Table Style Medium 13",
            "data": data,
            "columns": [
                {"header": DECISION_1},
                {"header": DECISION_2},
                {"header": DECISION_3},
                {"header": DECISION_4},
                {"header": DECISION_5},
                {"header": DECISION_6},
                {"header": DECISION_7},
                {"header": DECISION_8},
                {"header": DECISION_HUGE_BID},
                {"header": DECISION_HUGE_SPREAD},
                {"header": DECISION_HIGH_LIQUIDITY},
                {"header": DECISION_OTHER},
            ],
        },
    )


def _create_combined_yield_chartsheet(workbook, stats):
    chartsheet = workbook.add_chartsheet("Combined yield")
    chart = workbook.add_chart({"type": "column"})

    chart.set_title({"name": "Combined yield"})

    chart.set_x_axis({"visible": False})
    chart.set_y_axis({"visible": False})

    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!A1",
            "values": f"='Combined yield (Data)'!A2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!B1",
            "values": f"='Combined yield (Data)'!B2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!C1",
            "values": f"='Combined yield (Data)'!C2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!D1",
            "values": f"='Combined yield (Data)'!D2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!E1",
            "values": f"='Combined yield (Data)'!E2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!F1",
            "values": f"='Combined yield (Data)'!F2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!G1",
            "values": f"='Combined yield (Data)'!G2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!H1",
            "values": f"='Combined yield (Data)'!H2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!I1",
            "values": f"='Combined yield (Data)'!I2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!J1",
            "values": f"='Combined yield (Data)'!J2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!K1",
            "values": f"='Combined yield (Data)'!K2",
            "data_labels": {
                "value": True,
            },
        }
    )
    chart.add_series(
        {
            "name": f"='Combined yield (Data)'!L1",
            "values": f"='Combined yield (Data)'!L2",
            "data_labels": {
                "value": True,
            },
        }
    )

    chartsheet.set_chart(chart)


def _create_combined_positive_yield_sheet(workbook, stats):
    worksheet = workbook.add_worksheet("Combined positive yield (Data)")

    worksheet.set_column("A:A", 12.0)
    worksheet.set_column("B:B", 11.57)
    worksheet.set_column("C:C", 11.57)
    worksheet.set_column("D:D", 11.57)
    worksheet.set_column("E:E", 11.57)
    worksheet.set_column("F:F", 11.57)
    worksheet.set_column("G:G", 11.57)
    worksheet.set_column("H:H", 11.57)
    worksheet.set_column("I:I", 11.57)
    worksheet.set_column("J:J", 13.57)
    worksheet.set_column("K:K", 14.43)
    worksheet.set_column("L:L", 11.29)

    combined = stats["combined_positive"]
    data = [
        [
            combined[DECISION_1],
            combined[DECISION_2],
            combined[DECISION_3],
            combined[DECISION_4],
            combined[DECISION_5],
            combined[DECISION_6],
            combined[DECISION_7],
            combined[DECISION_8],
            combined[DECISION_HUGE_BID],
            combined[DECISION_HUGE_SPREAD],
            combined[DECISION_HIGH_LIQUIDITY],
            combined[DECISION_OTHER],
        ]
    ]

    worksheet.add_table(
        f"A1:L{len(data) + 1}",
        {
            "name": "CombinedPositiveYieldData",
            "style": "Table Style Medium 13",
            "data": data,
            "columns": [
                {"header": DECISION_1},
                {"header": DECISION_2},
                {"header": DECISION_3},
                {"header": DECISION_4},
                {"header": DECISION_5},
                {"header": DECISION_6},
                {"header": DECISION_7},
                {"header": DECISION_8},
                {"header": DECISION_HUGE_BID},
                {"header": DECISION_HUGE_SPREAD},
                {"header": DECISION_HIGH_LIQUIDITY},
                {"header": DECISION_OTHER},
            ],
        },
    )


def _create_combined_positive_yield_chartsheet(workbook, stats):
    chartsheet = workbook.add_chartsheet("Combined positive yield")
    chart = workbook.add_chart({"type": "pie"})

    chart.set_title({"name": "Combined positive yield"})

    chart.add_series(
        {
            "categories": f"='Combined positive yield (Data)'!A1:L1",
            "values": f"='Combined positive yield (Data)'!A2:L2",
            "data_labels": {
                "value": True,
            },
        }
    )

    chartsheet.set_chart(chart)


def _create_combined_negative_yield_sheet(workbook, stats):
    worksheet = workbook.add_worksheet("Combined negative yield (Data)")

    worksheet.set_column("A:A", 12.0)
    worksheet.set_column("B:B", 11.57)
    worksheet.set_column("C:C", 11.57)
    worksheet.set_column("D:D", 11.57)
    worksheet.set_column("E:E", 11.57)
    worksheet.set_column("F:F", 11.57)
    worksheet.set_column("G:G", 11.57)
    worksheet.set_column("H:H", 11.57)
    worksheet.set_column("I:I", 11.57)
    worksheet.set_column("J:J", 13.57)
    worksheet.set_column("K:K", 14.43)
    worksheet.set_column("L:L", 11.29)

    combined = stats["combined_negative"]
    data = [
        [
            combined[DECISION_1],
            combined[DECISION_2],
            combined[DECISION_3],
            combined[DECISION_4],
            combined[DECISION_5],
            combined[DECISION_6],
            combined[DECISION_7],
            combined[DECISION_8],
            combined[DECISION_HUGE_BID],
            combined[DECISION_HUGE_SPREAD],
            combined[DECISION_HIGH_LIQUIDITY],
            combined[DECISION_OTHER],
        ]
    ]

    worksheet.add_table(
        f"A1:L{len(data) + 1}",
        {
            "name": "CombinedNegativeYieldData",
            "style": "Table Style Medium 13",
            "data": data,
            "columns": [
                {"header": DECISION_1},
                {"header": DECISION_2},
                {"header": DECISION_3},
                {"header": DECISION_4},
                {"header": DECISION_5},
                {"header": DECISION_6},
                {"header": DECISION_7},
                {"header": DECISION_8},
                {"header": DECISION_HUGE_BID},
                {"header": DECISION_HUGE_SPREAD},
                {"header": DECISION_HIGH_LIQUIDITY},
                {"header": DECISION_OTHER},
            ],
        },
    )


def _create_combined_negative_yield_chartsheet(workbook, stats):
    chartsheet = workbook.add_chartsheet("Combined negative yield")
    chart = workbook.add_chart({"type": "pie"})

    chart.set_title({"name": "Combined negative yield"})

    chart.add_series(
        {
            "categories": f"='Combined negative yield (Data)'!A1:L1",
            "values": f"='Combined negative yield (Data)'!A2:L2",
            "data_labels": {
                "value": True,
            },
        }
    )

    chartsheet.set_chart(chart)


def _create_yield_per_decision_sheets(workbook, stats):
    i = 0

    for decision, decision_info in stats["yield_per_decision"].items():
        worksheet = workbook.add_worksheet(f"{decision} (Data)")

        worksheet.set_column("A:A", 62.14)
        worksheet.set_column("B:B", 23.0)

        instruments = [
            {
                "instrumentName": instrument_name,
                "yieldWithCommission": yield_with_commission,
            } for instrument_name, yield_with_commission in decision_info.items()
        ]
        instruments.sort(key=lambda x: x["yieldWithCommission"], reverse=True)

        data = []

        for entry in instruments:
            data.append(
                [
                    entry["instrumentName"],
                    entry["yieldWithCommission"],
                ]
            )

        worksheet.add_table(
            f"A1:B{len(data) + 1}",
            {
                "name": f"DecisionData{i}",
                "style": "Table Style Medium 13",
                "data": data,
                "columns": [
                    {"header": "Name"},
                    {"header": "Yield with commission"},
                ],
            },
        )

        i += 1


def _create_yield_per_decision_chartsheet(workbook, stats):
    for decision, decision_info in stats["yield_per_decision"].items():
        chartsheet = workbook.add_chartsheet(decision)
        chart = workbook.add_chart({"type": "bar"})

        chart.set_title({"name": decision})

        chart.set_x_axis({"visible": False})
        chart.set_y_axis({"visible": False})

        for i in reversed(range(len(decision_info))):
            chart.add_series(
                {
                    "name": f"='{decision} (Data)'!A{i + 2}",
                    "values": f"='{decision} (Data)'!B{i + 2}",
                    "data_labels": {
                        "value": True,
                    },
                }
            )

        chartsheet.set_chart(chart)


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

    match = decision_5_regexp.match(log_message)

    if match is not None:
        return DECISION_5

    match = decision_6_regexp.match(log_message)

    if match is not None:
        return DECISION_6

    match = decision_7_regexp.match(log_message)

    if match is not None:
        return DECISION_7

    match = decision_8_regexp.match(log_message)

    if match is not None:
        return DECISION_8

    match = huge_bid_regexp.match(log_message)

    if match is not None:
        return DECISION_HUGE_BID

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
