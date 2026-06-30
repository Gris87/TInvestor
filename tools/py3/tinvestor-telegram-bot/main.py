import asyncio
import argparse
import json
import logging
import os
import psutil
import re
import requests
import sys
import tempfile
import traceback
import uuid

from datetime import datetime
from telethon import TelegramClient, connection
from loguru import logger
from pathlib import Path


#logging.basicConfig(level=logging.DEBUG)


mtproto_regexp = re.compile(r'https:\/\/t.me\/proxy\?server=(.+)&port=(\d+)&secret=(.+)')


def telegram_bot(args, filter):
    _check_running_instance()
    _check_silence_range(args)

    api_id = os.environ["TELEGRAM_API_ID"]
    api_hash = os.environ["TELEGRAM_API_HASH"]
    mtproxy_server = os.environ.get("TELEGRAM_MTPROXY_SERVER", "")
    mtproxy_port = int(os.environ.get("TELEGRAM_MTPROXY_PORT", 0))
    mtproxy_secret = os.environ.get("TELEGRAM_MTPROXY_SECRET", "")
    bot_token = os.environ["TELEGRAM_TOKEN"]

    if mtproxy_server == "" or mtproxy_port == 0 or mtproxy_secret == "":
        mtproxy_server, mtproxy_port, mtproxy_secret = _get_mtproto(args)

        if mtproxy_server == "" or mtproxy_port == 0 or mtproxy_secret == "":
            logger.error("Failed to get MTPROTO")

            sys.exit(1)

    temp_uuid = uuid.uuid4()
    temp_file_session=f"/tmp/tinvestor-telegram-bot-{temp_uuid}"

    Path(f"{temp_file_session}.session").unlink(missing_ok=True)
    Path(f"{temp_file_session}.session-journal").unlink(missing_ok=True)

    res = True

    try:
        client = TelegramClient(
            temp_file_session,
            api_id,
            api_hash,
            connection=connection.ConnectionTcpMTProxyRandomizedIntermediate if mtproxy_server != "" else connection.ConnectionTcpFull,
            proxy=(mtproxy_server, mtproxy_port, mtproxy_secret) if mtproxy_server != "" else None
        ).start(bot_token=bot_token)

        with client:
            client.loop.run_until_complete(_process_files(args, client, filter))
    except Exception as e:
        res = False

        traceback.print_exc()

    Path(f"{temp_file_session}.session").unlink(missing_ok=True)
    Path(f"{temp_file_session}.session-journal").unlink(missing_ok=True)

    return res


def _check_running_instance():
    found = False

    for p in psutil.process_iter(["name", "cmdline"]):
        if "python" in p.info["name"]:
            cmdline = p.info["cmdline"]

            if cmdline is not None and len(cmdline) > 1 and "tools/py3/tinvestor-telegram-bot" in cmdline[1] and p.pid != os.getpid():
                found = True

                break

    if found:
        logger.warning("Another instance is running. Terminating")

        sys.exit(0)


def _check_silence_range(args):
    if args.silence != "":
        ranges = args.silence.split("-")

        if len(ranges) != 2:
            logger.error("Please specify valid silence range")

            sys.exit(1)

        start_time = datetime.strptime(ranges[0], "%H:%M").time()
        end_time   = datetime.strptime(ranges[1], "%H:%M").time()
        now        = datetime.now().time()

        if start_time < end_time:
            if now >= start_time and now < end_time:
                logger.warning("Zzzzzz")

                sys.exit(0)
        else:
            if now >= start_time or now < end_time:
                logger.warning("Zzzzzz")

                sys.exit(0)


def _get_mtproto(args):
    res_server = ""
    res_port = 0
    res_secret = ""

    api_id = os.environ["TELEGRAM_API_ID"]
    api_hash = os.environ["TELEGRAM_API_HASH"]
    bot_token = os.environ["TELEGRAM_TOKEN"]

    cache_folder_path = Path(args.cache) / "telegram"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    proxies_path = cache_folder_path / "all_proxies.json"
    old_proxies = []

    if proxies_path.exists():
        with open(proxies_path, "r", encoding="utf-8") as f:
            old_proxies = json.loads(f.read())

    logger.info("Getting all_proxies.txt from GitHub")
    response = requests.get("https://raw.githubusercontent.com/SoliSpirit/mtproto/refs/heads/master/all_proxies.txt")

    proxies = list(filter(None, response.text.split("\n")))
    new_proxies = list(dict.fromkeys(old_proxies + proxies))

    index = 0

    while index < len(new_proxies):
        logger.info(f"Trying MTPROTO {new_proxies[index]}")

        match = mtproto_regexp.match(new_proxies[index])

        if match is not None:
            mtproxy_server = match.group(1)
            mtproxy_port = int(match.group(2))
            mtproxy_secret = match.group(3)

            temp_uuid = uuid.uuid4()
            temp_file_session=f"/tmp/tinvestor-telegram-bot-{temp_uuid}"

            Path(f"{temp_file_session}.session").unlink(missing_ok=True)
            Path(f"{temp_file_session}.session-journal").unlink(missing_ok=True)

            good = True

            try:
                client = TelegramClient(
                    temp_file_session,
                    api_id,
                    api_hash,
                    connection=connection.ConnectionTcpMTProxyRandomizedIntermediate,
                    proxy=(mtproxy_server, mtproxy_port, mtproxy_secret),
                    timeout=1,
                    connection_retries=0
                ).start(bot_token=bot_token)

                with client:
                    client.loop.run_until_complete(_dummy())
            except Exception as e:
                good = False

            Path(f"{temp_file_session}.session").unlink(missing_ok=True)
            Path(f"{temp_file_session}.session-journal").unlink(missing_ok=True)

            if good:
                res_server = mtproxy_server
                res_port = mtproxy_port
                res_secret = mtproxy_secret

                break
        else:
            logger.warning("Failed to parse MTPROTO {new_proxies[index]}")

        index += 1

    new_proxies = new_proxies[index:]

    with open(proxies_path, "w", encoding="utf-8") as f:
        json.dump(new_proxies, f, ensure_ascii=False)

    return res_server, res_port, res_secret


async def _dummy():
    pass


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
        await _send_message(client, record)


async def _send_message(client, record):
    data=record["data"]
    text=record["text"]

    logger.info(f"Send message: {text}")

    target_username = os.environ["TELEGRAM_TARGET_USERNAME"]

    if data != "":
        with tempfile.TemporaryDirectory() as tmp_dir_name:
            with open(f"{tmp_dir_name}/log.txt", "w") as f:
                f.write(data)

            await client.send_file(target_username, file=f"{tmp_dir_name}/log.txt", caption=text)
    else:
        await client.send_message(target_username, text)


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
        "--silence",
        dest="silence",
        type=str,
        default="",
        help="Time range of silence in format HH:MM-HH:MM"
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
