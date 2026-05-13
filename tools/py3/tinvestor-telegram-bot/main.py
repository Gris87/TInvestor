import asyncio
import argparse
import json
import logging
import os
import sys
import traceback

from telethon import TelegramClient, connection
from loguru import logger
from pathlib import Path


#logging.basicConfig(level=logging.DEBUG)


def telegram_bot(args, filter):
    api_id = os.environ["TELEGRAM_API_ID"]
    api_hash = os.environ["TELEGRAM_API_HASH"]
    mtproxy_server = os.environ["TELEGRAM_MTPROXY_SERVER"]
    mtproxy_port = int(os.environ["TELEGRAM_MTPROXY_PORT"])
    mtproxy_secret = os.environ["TELEGRAM_MTPROXY_SECRET"]
    bot_token = os.environ["TELEGRAM_TOKEN"]

    Path("bot.session").unlink(missing_ok=True)
    Path("bot.session-journal").unlink(missing_ok=True)

    res = True

    try:
        client = TelegramClient("bot", api_id, api_hash, connection=connection.ConnectionTcpMTProxyRandomizedIntermediate, proxy=(mtproxy_server, mtproxy_port, mtproxy_secret)).start(bot_token=bot_token)

        with client:
            client.loop.run_until_complete(_process_files(args, client, filter))
    except Exception as e:
        res = False

        traceback.print_exc()

    Path("bot.session").unlink(missing_ok=True)
    Path("bot.session-journal").unlink(missing_ok=True)

    return res


async def _process_files(args, client, filter):
    cache_folder_path = Path(args.cache) / "telegram"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    state_path = cache_folder_path / "state.json"

    last_timestamp = 0

    if state_path.exists():
        with open(state_path, "r", encoding="utf-8") as f:
            old_state = json.loads(f.read())

        last_timestamp = old_state["last_timestamp"]

    for record_path in sorted(Path(args.path_to_notifications).rglob("*.json"), key=lambda x: x.stat().st_mtime):
        if record_path.stat().st_mtime >= last_timestamp:
            last_timestamp = record_path.stat().st_mtime

            await _process_file(args, client, filter, record_path)

    state = {
        "last_timestamp": last_timestamp + 1
    }

    with open(state_path, "w", encoding="utf-8") as f:
        json.dump(state, f, ensure_ascii=False)


async def _process_file(args, client, filter, record_path):
    with open(record_path, "r", encoding="utf-8") as f:
        record = json.loads(f.read())

    if args.filter == "all" or record["type"] in filter:
        await _send_message(client, record["text"])


async def _send_message(client, msg):
    logger.info(f"Send message: {msg}")

    target_username = os.environ["TELEGRAM_TARGET_USERNAME"]

    await client.send_message(target_username, msg)


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--cache",
        dest="cache",
        type=str,
        default="",
        help="Path to cache folder"
    )
    parser.add_argument(
        "--path-to-notifications-path",
        dest="path_to_notifications",
        type=str,
        default="",
        help="Path to notifications folder"
    )
    parser.add_argument(
        "--filter",
        dest="filter",
        type=str,
        default="all",
        help="Comma separated list of filters: system,portfolio,huge_sell,dividends,pulse_neutral,pulse_buy,pulse_sell (default: all)"
    )
    args = parser.parse_args()

    expected_env_vars = [
        "TELEGRAM_API_ID",
        "TELEGRAM_API_HASH",
        "TELEGRAM_MTPROXY_SERVER",
        "TELEGRAM_MTPROXY_PORT",
        "TELEGRAM_MTPROXY_SECRET",
        "TELEGRAM_TOKEN",
        "TELEGRAM_TARGET_USERNAME"
    ]
    good = True

    for var in expected_env_vars:
        if var not in os.environ:
            logger.error(f"Environment variable {var} is not set properly")

            good = False

    if not good:
        sys.exit(1)

    if args.cache == "":
        logger.error("Please specify path to cache folder with --cache")

        sys.exit(1)

    if args.path_to_notifications == "":
        logger.error("Please specify path to notifications folder with --path-to-notifications-path")

        sys.exit(1)

    filter = args.filter.split(",")

    for f in filter:
        if f not in ["system", "portfolio", "huge_sell", "dividends", "pulse_neutral", "pulse_buy", "pulse_sell", "all"]:
            logger.error("Please specify valid filter")

            sys.exit(1)

    sys.exit(0 if telegram_bot(args, filter) else 1)
