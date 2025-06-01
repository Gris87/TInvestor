import argparse
import json
import sys
from loguru import logger

from pathlib import Path

from tinkoff.invest import Client, GetOperationsByCursorRequest
from tinkoff.invest.constants import INVEST_GRPC_API, INVEST_GRPC_API_SANDBOX
from tinkoff.invest.schemas import OperationState


def collect_tinkoff_data(args):
    Path(args.output).mkdir(parents=True, exist_ok=True)

    logger.info("Connecting to server")

    with Client(args.token, target=INVEST_GRPC_API if args.official else INVEST_GRPC_API_SANDBOX) as client:
        accounts, portfolios, operations = _collect_data(client)

        with open(f"{args.output}/accounts.json", "w", encoding='utf-8') as f:
            f.write(json.dumps(accounts, indent=4, ensure_ascii=False))

        for i, portfolio_by_account in enumerate(portfolios):
            with open(f"{args.output}/portfolio_{i}.json", "w", encoding='utf-8') as f:
                f.write(json.dumps(portfolio_by_account, indent=4, ensure_ascii=False))

        for i, operations_by_account in enumerate(operations):
            with open(f"{args.output}/operations_{i}.json", "w", encoding='utf-8') as f:
                f.write(json.dumps(operations_by_account, indent=4, ensure_ascii=False))


def _collect_data(client):
    logger.info("Collecting data")

    accounts = _get_accounts(client)
    portfolios = _get_portfolio(client, accounts)
    operations = _get_operations(client, accounts)

    return accounts, portfolios, operations


def _get_accounts(client):
    res = []

    for account in client.users.get_accounts().accounts:
        account_dict = account.__dict__

        account_dict["type"] = account_dict["type"].name
        account_dict["status"] = account_dict["status"].name
        account_dict["opened_date"] = account_dict["opened_date"].isoformat()
        account_dict["closed_date"] = account_dict["closed_date"].isoformat()
        account_dict["access_level"] = account_dict["access_level"].name

        res.append(account_dict)

    return res


def _get_portfolio(client, accounts):
    res = []

    for account in accounts:
        portfolio_dict = client.operations.get_portfolio(account_id=account["id"]).__dict__

        portfolio_dict["total_amount_shares"] = portfolio_dict["total_amount_shares"].__dict__
        portfolio_dict["total_amount_bonds"] = portfolio_dict["total_amount_bonds"].__dict__
        portfolio_dict["total_amount_etf"] = portfolio_dict["total_amount_etf"].__dict__
        portfolio_dict["total_amount_currencies"] = portfolio_dict["total_amount_currencies"].__dict__
        portfolio_dict["total_amount_futures"] = portfolio_dict["total_amount_futures"].__dict__
        portfolio_dict["expected_yield"] = portfolio_dict["expected_yield"].__dict__
        portfolio_dict["total_amount_options"] = portfolio_dict["total_amount_options"].__dict__
        portfolio_dict["total_amount_sp"] = portfolio_dict["total_amount_sp"].__dict__
        portfolio_dict["total_amount_portfolio"] = portfolio_dict["total_amount_portfolio"].__dict__
        portfolio_dict["daily_yield"] = portfolio_dict["daily_yield"].__dict__
        portfolio_dict["daily_yield_relative"] = portfolio_dict["daily_yield_relative"].__dict__

        positions = portfolio_dict["positions"]

        for i in range(len(positions)):
            position_dict = positions[i].__dict__

            position_dict["quantity"] = position_dict["quantity"].__dict__
            position_dict["average_position_price"] = position_dict["average_position_price"].__dict__
            position_dict["expected_yield"] = position_dict["expected_yield"].__dict__
            position_dict["current_nkd"] = position_dict["current_nkd"].__dict__
            position_dict["average_position_price_pt"] = position_dict["average_position_price_pt"].__dict__
            position_dict["current_price"] = position_dict["current_price"].__dict__
            position_dict["average_position_price_fifo"] = position_dict["average_position_price_fifo"].__dict__
            position_dict["quantity_lots"] = position_dict["quantity_lots"].__dict__
            position_dict["blocked_lots"] = position_dict["blocked_lots"].__dict__
            position_dict["var_margin"] = position_dict["var_margin"].__dict__
            position_dict["expected_yield_fifo"] = position_dict["expected_yield_fifo"].__dict__
            position_dict["daily_yield"] = position_dict["daily_yield"].__dict__

            positions[i] = position_dict

        portfolio_dict["positions"] = positions

        virtual_positions = portfolio_dict["virtual_positions"]

        for i in range(len(virtual_positions)):
            position_dict = virtual_positions[i].__dict__

            position_dict["quantity"] = position_dict["quantity"].__dict__
            position_dict["average_position_price"] = position_dict["average_position_price"].__dict__
            position_dict["expected_yield"] = position_dict["expected_yield"].__dict__
            position_dict["expected_yield_fifo"] = position_dict["expected_yield_fifo"].__dict__
            position_dict["expire_date"] = position_dict["expire_date"].isoformat()
            position_dict["current_price"] = position_dict["current_price"].__dict__
            position_dict["average_position_price_fifo"] = position_dict["average_position_price_fifo"].__dict__
            position_dict["daily_yield"] = position_dict["daily_yield"].__dict__

            virtual_positions[i] = position_dict

        portfolio_dict["virtual_positions"] = virtual_positions

        res.append(portfolio_dict)

    return res


def _get_operations(client, accounts):
    res = []

    for account in accounts:
        group = []
        cursor = ""

        while True:
            req = GetOperationsByCursorRequest(
                account_id=account["id"],
                limit=1000,
                state=OperationState.OPERATION_STATE_EXECUTED,
                cursor=cursor,
                without_trades=True
            )

            operations_dict = client.operations.get_operations_by_cursor(req).__dict__

            items = operations_dict["items"]

            for i in range(len(items)):
                item_dict = items[i].__dict__

                item_dict["date"] = item_dict["date"].isoformat()
                item_dict["type"] = item_dict["type"].name
                item_dict["state"] = item_dict["state"].name
                item_dict["instrument_kind"] = item_dict["instrument_kind"].name
                item_dict["payment"] = item_dict["payment"].__dict__
                item_dict["price"] = item_dict["price"].__dict__
                item_dict["commission"] = item_dict["commission"].__dict__
                item_dict["yield_"] = item_dict["yield_"].__dict__
                item_dict["yield_relative"] = item_dict["yield_relative"].__dict__
                item_dict["accrued_int"] = item_dict["accrued_int"].__dict__
                item_dict["cancel_date_time"] = item_dict["cancel_date_time"].isoformat()
                item_dict["trades_info"] = item_dict["trades_info"].__dict__

                items[i] = item_dict

            operations_dict["items"] = items

            group.append(operations_dict)

            if not operations_dict["has_next"]:
                break

            cursor = operations_dict["next_cursor"]

        res.append(group)

    return res


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--official",
        default=False,
        action="store_true",
        help="Flag for using official server",
    )
    parser.add_argument(
        "--token",
        dest="token",
        type=str,
        default="t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ",
        help="Token for Tinkoff API",
    )
    parser.add_argument(
        "--output",
        dest="output",
        type=str,
        default="build",
        help="Output directory",
    )
    args = parser.parse_args()

    sys.exit(0 if collect_tinkoff_data(args) else 1)
