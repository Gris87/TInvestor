import asyncio
import argparse
import logging
import sys
from loguru import logger

from tinkoff.invest.constants import INVEST_GRPC_API
from tinkoff.invest.retrying.aio.client import AsyncRetryingClient
from tinkoff.invest.retrying.settings import RetryClientSettings
from tinkoff.invest.schemas import CreateFavoriteGroupRequest, DeleteFavoriteGroupRequest, EditFavoritesActionType, EditFavoritesRequestInstrument, GetFavoriteGroupsRequest

from localization import *


#logging.basicConfig(level=logging.DEBUG)


LNZL_UID   = "4563f7a1-8245-4caf-aba5-ac49827ba775"
LNZLP_UID  = "28fdec79-fcf0-40cb-b53c-586179f024e5"
GTRK_UID   = "9e69afb6-4561-4fc2-b63b-b181e3f9ecdc"
TMON_UID   = "498ec3ff-ef27-4729-9703-a5aac48d5789"
IMOEXF_UID = "5bcff194-f10d-4314-b9ee-56b7fdb344fd"

IGNORE_STOCKS = [
    LNZL_UID,
    LNZLP_UID,
    GTRK_UID
]

EXTEND_STOCKS = [
    TMON_UID,
    IMOEXF_UID
]


async def favorite_group(args):
    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API) as client:
        logger.info("Obtaining favorite groups")

        groups = await _get_favorite_groups(client)

        for groupName in groups.keys():
            logger.info(f"- {groupName}")

        logger.info("Obtaining stocks")

        stocks = await _get_stocks(client)
        logger.info(f"{len(stocks)} stocks found")

        logger.info("Creating favorite group")

        await _create_favorite_group(client, groups, stocks)

    return


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


async def _get_favorite_groups(client):
    res = {}

    req = GetFavoriteGroupsRequest()
    tinkoff_groups = await client.instruments.get_favorite_groups(req)

    for group in tinkoff_groups.groups:
        res[group.group_name] = group

    return res


async def _get_stocks(client):
    res = []

    tinkoff_shares = await client.instruments.shares()

    for instrument in tinkoff_shares.instruments:
        if instrument.currency == "rub" and instrument.api_trade_available_flag and instrument.uid not in IGNORE_STOCKS:
            res.append(instrument.uid)

    res.extend(EXTEND_STOCKS)

    return res


async def _create_favorite_group(client, groups, stocks):
    if favorite_group_name in groups:
        req = DeleteFavoriteGroupRequest(group_id=groups[favorite_group_name].group_id)
        await client.instruments.delete_favorite_group(req)

    req = CreateFavoriteGroupRequest(group_name=favorite_group_name, group_color="FFFFFF")
    resp = await client.instruments.create_favorite_group(req)

    groupId = resp.group_id

    for i, instrumentId in enumerate(stocks):
        await client.instruments.edit_favorites(
            instruments=[EditFavoritesRequestInstrument(instrument_id=instrumentId)],
            action_type=EditFavoritesActionType.EDIT_FAVORITES_ACTION_TYPE_ADD,
            group_id=groupId
        )
        logger.info(f"{i+1} of {len(stocks)} added")

        await asyncio.sleep(5)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--token",
        dest="token",
        type=str,
        default="",
        help="Token for Tinkoff API",
    )
    parser.add_argument(
        "--token-file",
        dest="token_file",
        type=str,
        default="",
        help="Path to file with token for Tinkoff API",
    )
    args = parser.parse_args()

    if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
        logger.error("Please specify token with --token or --token-file")

        sys.exit(1)

    asyncio.run(favorite_group(args))
