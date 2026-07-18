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


NEW_APRI_UID = "1c5fd815-eb3e-41b1-a5b7-9cc7c905bd78"
NEW_ARSA_UID = "786c1d68-0864-4804-b3ca-2166766cd917"
NEW_ASSB_UID = "578506fc-187d-488c-a10e-2651530da4b9"
NEW_AVAN_UID = "e027aaaa-cb20-4d48-be23-6d2d4b0edfb8"
NEW_BISVP_UID = "56452fa0-1e58-457e-bb41-90acaabf0767"
NEW_BRZL_UID = "5a6c9d26-10db-4a21-9c0e-2b7b063c69f8"
NEW_BSPBP_UID = "e71d6238-71ea-400c-9451-edc7b3338a2a"
NEW_CHGZ_UID = "5d06ba3a-408a-410e-aea1-41d1adf81db3"
NEW_CHKZ_UID = "5f858c64-ba9a-4d66-86bf-73b5880cf30f"
NEW_DIOD_UID = "464c9ca5-2ba5-4b66-90e2-005f30bff134"
NEW_DZRDP_UID = "c4304c3c-4106-47ea-aabc-e1ae39930791"
NEW_DZRD_UID = "47dc8658-0d02-4429-b75e-02bdf465c22f"
NEW_EELT_UID = "1c181853-2fd6-4a20-a1c6-cb923a7c7919"
NEW_EVRZ_UID = "ec010d2f-a4e1-463a-816a-c00b9b044345"
NEW_GAZAP_UID = "09c5f10f-b671-4acc-bc17-db491a1a66eb"
NEW_GAZA_UID = "fe427a08-8878-493d-b4cf-8ba65dfb8454"
NEW_GEMA_UID = "0f446a59-c78e-44fc-8235-7b097096769a"
NEW_HIMCP_UID = "c53f1d8f-78e0-46e5-ac31-fdc08f05c8e2"
NEW_IGSTP_UID = "0c275361-5f48-4138-a592-6da7d275075d"
NEW_IGST_UID = "641576a5-9abd-47fa-99c5-7380c8890355"
NEW_JNOSP_UID = "e79f65d6-1bd0-42c6-943e-5060bbeabc45"
NEW_JNOS_UID = "5fdf1499-288c-4b0e-8d4a-10c5f5fefb57"
NEW_KBSB_UID = "104c0979-1f16-4e13-9787-dc767814c4a6"
NEW_KCHEP_UID = "ef832fa3-1d62-4a18-b3ca-776d76a29cb1"
NEW_KCHE_UID = "caa615ad-99c1-499c-b592-56368f2ebd66"
NEW_KFBA_UID = "0bdca049-f06d-4a27-b46c-f89236a44727"
NEW_KMEZ_UID = "e3e10315-c9e8-452a-9113-2f14805679ce"
NEW_KOGK_UID = "b4d1ec34-82ef-4a47-a639-67a8ab8b683e"
NEW_KRKN_UID = "3d83aedf-b48f-4791-9370-c0569170a82d"
NEW_KRKOP_UID = "c961e321-503a-442c-8c4a-5dce95d75513"
NEW_KROTP_UID = "205b66cd-fa09-4fbd-85c3-46eefc9eff40"
NEW_KRSBP_UID = "241dc583-9e35-4b58-8bee-632691c8b4b8"
NEW_KRSB_UID = "d8de3aca-92e5-4135-8f1d-c3d545cbc041"
NEW_KUZB_UID = "879b2e35-edfb-42f4-baa0-8a00cf1edaac"
NEW_LMBZ_UID = "ee64af27-d3f9-4a9a-a50a-7143c6087371"
NEW_LPSB_UID = "b8d26943-a3c1-465a-8c58-518466e2ab26"
NEW_LVHK_UID = "76558b5d-9875-4c92-82ca-44f2d0b39e1b"
NEW_MAGEP_UID = "b4959236-ef80-4eec-a5e0-44cc423b6ab0"
NEW_MAGE_UID = "1214a841-5a72-42ac-b911-5c05a7eaa48e"
NEW_MFGSP_UID = "1c0d371b-1b75-4185-8877-45da35cbd692"
NEW_MFGS_UID = "df35ac3e-3d77-454f-b446-cdbb9d867b02"
NEW_MGTS_UID = "d614f6be-89a7-4fda-86f4-580f8d7463ae"
NEW_MISBP_UID = "97b4c319-1a9e-4550-a9f1-c39b489713ba"
NEW_MISB_UID = "bd685d6d-715f-499b-a50d-3b47c04a65a6"
NEW_MRKK_UID = "49b8a647-e740-4c29-b390-4d1d6829d13a"
NEW_MRSB_UID = "1037b19b-46f4-4ebf-bb87-53401f8d2148"
NEW_NAUK_UID = "700671c4-624e-43d0-8377-95de3956b111"
NEW_NFAZ_UID = "7e13d581-bbbf-41bb-89c9-e2c4a85a9bde"
NEW_NKSH_UID = "2ff42bea-3ea9-4af9-a06e-19fe0874b7a1"
NEW_NNSBP_UID = "9e16009b-ec7a-499a-b0ce-9d75f9b69117"
NEW_NNSB_UID = "9b5366b0-2a2e-4074-ace6-d844fefee42b"
NEW_OMZZP_UID = "f26aade9-d33a-4cdf-867e-1c18c753e63a"
NEW_PAZA_UID = "5a4b9636-2445-43db-a298-9dada56ba3e8"
NEW_PRMB_UID = "c0545aeb-b6b1-4ccb-b37e-3eaa427b4717"
NEW_RDRB_UID = "8d83880d-722a-45cd-b4e3-1a7c95112ed5"
NEW_RGSS_UID = "e3312603-d109-443a-86b5-329eede6c853"
NEW_ROST_UID = "e53026a3-5585-478a-8ae7-59698864f681"
NEW_RTGZ_UID = "210ef34f-e815-47a2-85c9-d2a033fc642f"
NEW_RTSBP_UID = "2113c648-6459-49c5-bdfc-3c608d2afaa0"
NEW_RTSB_UID = "deab934f-7e5e-4467-a465-3624c1110e7b"
NEW_RUSI_UID = "01fd2cc9-489c-4756-b267-72c3ccc61b8f"
NEW_RZSB_UID = "a2284e69-0e1e-4fdd-8cc9-5d991d936b90"
NEW_SAGOP_UID = "335cb2e2-6922-42a3-803b-60024cf79ce1"
NEW_SAGO_UID = "fb46349d-a7ef-413b-917f-edec1a1529cc"
NEW_SAREP_UID = "9bb7b964-285f-43c6-ba61-1dc829f25e30"
NEW_SARE_UID = "b8c992db-6cb8-449e-8602-bb4252a28ea4"
NEW_SLEN_UID = "813fac1c-eb17-4ccb-9ec3-72a80814fc85"
NEW_STSBP_UID = "6bffd7f3-4c01-495b-9a4d-e1f8c9e1774f"
NEW_STSB_UID = "e268b9d4-7c8c-44b4-b2b2-11c0d55155e8"
NEW_SVETP_UID = "edc6b309-746d-4697-9752-b7697b0ba38a"
NEW_SVET_UID = "6fdba807-39e5-41d8-a272-ca2e7e35223d"
NEW_TASBP_UID = "d61f5ab1-ec03-4dc8-9f10-2f746b0a20c1"
NEW_TASB_UID = "54e856d4-ba8a-4365-8fd1-245a8888b306"
NEW_TNSE_UID = "703e944b-d4e6-4697-bea5-844c2c8be554"
NEW_TORSP_UID = "2d98aa0a-dfdb-42e9-8762-9db3eab40152"
NEW_TORS_UID = "414928c5-2ae2-4dff-9420-5f1c85146a99"
NEW_TUZA_UID = "4744e309-4b10-48c7-8dc8-c6ad1ccf5c3b"
NEW_UKUZ_UID = "cf07cbaa-5ef6-41f1-abc3-8d12d4bc6edf"
NEW_URKZ_UID = "9ec09704-0912-4ab8-9ebf-3fdad6fd3944"
NEW_USBN_UID = "71e9e3f7-60b6-4ec5-ae49-d4eba51d648e"
NEW_UTAR_UID = "2ca51950-2047-4090-b6ba-6ecd486291a2"
NEW_VGSBP_UID = "7b1e51e2-9de2-47d6-bf3d-ab502ee54df8"
NEW_VGSB_UID = "2bd81af0-b389-4638-ac9b-35a9938be0cc"
NEW_VJGZP_UID = "1b3363dd-8c0e-4d67-abf2-0666ce5a23f5"
NEW_VJGZ_UID = "32822b7c-b13c-45df-b1af-cb6e3d676399"
NEW_VLHZ_UID = "784f9e60-4214-4730-ba21-ddb2dd677a8f"
NEW_VRSBP_UID = "806938cd-172e-477f-b00c-c8e731cfafa7"
NEW_VSYDP_UID = "c6fd4e8e-5bb4-4f8a-9727-5d8dddff2e7d"
NEW_VSYD_UID = "4c5774d2-a1c6-4069-860b-3a93db0b4931"
NEW_WTCMP_UID = "d6461eb6-6557-4993-8d26-6202a1820667"
NEW_WTCM_UID = "f06a87cb-6a2f-42be-85ea-6698d3a62037"
NEW_YKENP_UID = "d69ebc99-fafb-4f82-af9c-b52aa49da613"
NEW_YKEN_UID = "1ed313a9-c6d6-4359-9cf2-e2d7011de00e"
NEW_YRSBP_UID = "97015e2f-adae-4133-9ce0-cd79015d13a2"
NEW_YRSB_UID = "7e669c8c-ed3e-4f86-a255-8af223f9b824"
NEW_ZILL_UID = "a257d925-305e-43e2-a9d5-1174c1190dee"
NEW_ZVEZ_UID = "c35cfe95-ad55-44ab-84d4-2e9754bb55b4"

GTRK_UID = "9e69afb6-4561-4fc2-b63b-b181e3f9ecdc"
JETL_UID = "34f51a49-919b-4eb1-9ed0-59c26f4cdc1e"

TMON_UID   = "498ec3ff-ef27-4729-9703-a5aac48d5789"
IMOEXF_UID = "5bcff194-f10d-4314-b9ee-56b7fdb344fd"

LOCKED_STOCKS = [
    NEW_APRI_UID,
    NEW_ARSA_UID,
    NEW_ASSB_UID,
    NEW_AVAN_UID,
    NEW_BISVP_UID,
    NEW_BRZL_UID,
    NEW_BSPBP_UID,
    NEW_CHGZ_UID,
    NEW_CHKZ_UID,
    NEW_DIOD_UID,
    NEW_DZRDP_UID,
    NEW_DZRD_UID,
    NEW_EELT_UID,
    NEW_EVRZ_UID,
    NEW_GAZAP_UID,
    NEW_GAZA_UID,
    NEW_GEMA_UID,
    NEW_HIMCP_UID,
    NEW_IGSTP_UID,
    NEW_IGST_UID,
    NEW_JNOSP_UID,
    NEW_JNOS_UID,
    NEW_KBSB_UID,
    NEW_KCHEP_UID,
    NEW_KCHE_UID,
    NEW_KFBA_UID,
    NEW_KMEZ_UID,
    NEW_KOGK_UID,
    NEW_KRKN_UID,
    NEW_KRKOP_UID,
    NEW_KROTP_UID,
    NEW_KRSBP_UID,
    NEW_KRSB_UID,
    NEW_KUZB_UID,
    NEW_LMBZ_UID,
    NEW_LPSB_UID,
    NEW_LVHK_UID,
    NEW_MAGEP_UID,
    NEW_MAGE_UID,
    NEW_MFGSP_UID,
    NEW_MFGS_UID,
    NEW_MGTS_UID,
    NEW_MISBP_UID,
    NEW_MISB_UID,
    NEW_MRKK_UID,
    NEW_MRSB_UID,
    NEW_NAUK_UID,
    NEW_NFAZ_UID,
    NEW_NKSH_UID,
    NEW_NNSBP_UID,
    NEW_NNSB_UID,
    NEW_OMZZP_UID,
    NEW_PAZA_UID,
    NEW_PRMB_UID,
    NEW_RDRB_UID,
    NEW_RGSS_UID,
    NEW_ROST_UID,
    NEW_RTGZ_UID,
    NEW_RTSBP_UID,
    NEW_RTSB_UID,
    NEW_RUSI_UID,
    NEW_RZSB_UID,
    NEW_SAGOP_UID,
    NEW_SAGO_UID,
    NEW_SAREP_UID,
    NEW_SARE_UID,
    NEW_SLEN_UID,
    NEW_STSBP_UID,
    NEW_STSB_UID,
    NEW_SVETP_UID,
    NEW_SVET_UID,
    NEW_TASBP_UID,
    NEW_TASB_UID,
    NEW_TNSE_UID,
    NEW_TORSP_UID,
    NEW_TORS_UID,
    NEW_TUZA_UID,
    NEW_UKUZ_UID,
    NEW_URKZ_UID,
    NEW_USBN_UID,
    NEW_UTAR_UID,
    NEW_VGSBP_UID,
    NEW_VGSB_UID,
    NEW_VJGZP_UID,
    NEW_VJGZ_UID,
    NEW_VLHZ_UID,
    NEW_VRSBP_UID,
    NEW_VSYDP_UID,
    NEW_VSYD_UID,
    NEW_WTCMP_UID,
    NEW_WTCM_UID,
    NEW_YKENP_UID,
    NEW_YKEN_UID,
    NEW_YRSBP_UID,
    NEW_YRSB_UID,
    NEW_ZILL_UID,
    NEW_ZVEZ_UID
]

IGNORE_STOCKS = [
    GTRK_UID,
    JETL_UID
]

EXTEND_STOCKS = [
    TMON_UID,
    IMOEXF_UID
]


async def favorite_group(args):
    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10000)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API) as client:
        logger.info("Obtaining user qual investor status")

        qual_investor = await _get_qual_investor(client)

        logger.info("Obtaining favorite groups")

        groups = await _get_favorite_groups(client)

        for groupName in groups.keys():
            logger.info(f"- {groupName}")

        logger.info("Obtaining stocks")

        stocks = await _get_stocks(client, qual_investor)
        logger.info(f"{len(stocks)} stocks found")

        logger.info("Creating favorite group")

        await _create_favorite_group(client, groups, stocks)

    return


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


async def _get_qual_investor(client):
    user_info = await client.users.get_info()

    return user_info.qual_status


async def _get_favorite_groups(client):
    res = {}

    req = GetFavoriteGroupsRequest()
    tinkoff_groups = await client.instruments.get_favorite_groups(req)

    for group in tinkoff_groups.groups:
        res[group.group_name] = group

    return res


async def _get_stocks(client, qual_investor):
    res = []

    tinkoff_shares = await client.instruments.shares()

    for instrument in tinkoff_shares.instruments:
        if instrument.currency == "rub" and instrument.api_trade_available_flag and instrument.buy_available_flag and instrument.sell_available_flag and (qual_investor or (not instrument.for_qual_investor_flag and instrument.uid not in LOCKED_STOCKS)) and instrument.uid not in IGNORE_STOCKS:
            res.append(instrument.uid)

    res.extend(EXTEND_STOCKS)

    return res


async def _create_favorite_group(client, groups, stocks):
    if favorite_group_name in groups:
        req = DeleteFavoriteGroupRequest(group_id=groups[favorite_group_name].group_id)
        await client.instruments.delete_favorite_group(req)

    req = CreateFavoriteGroupRequest(group_name=favorite_group_name, group_color="FFFFFF")
    resp = await client.instruments.create_favorite_group(req)

    group_id = resp.group_id

    for i, instrumentId in enumerate(stocks):
        logger.info(f"{i+1}/{len(stocks)} - {instrumentId}")
        await client.instruments.edit_favorites(
            instruments=[EditFavoritesRequestInstrument(instrument_id=instrumentId)],
            action_type=EditFavoritesActionType.EDIT_FAVORITES_ACTION_TYPE_ADD,
            group_id=group_id
        )


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
