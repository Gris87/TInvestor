import argparse
import json
import requests
import sys
import time
from loguru import logger

from localization import *


TELEGRAM_TOKEN = "8347815362:AAH2Xa8Mujy7OBsyJm_FWjZJeZekYaIABns"
TELEGRAM_SEND_URL = f"https://api.telegram.org/bot{TELEGRAM_TOKEN}/sendMessage"

OPERATIONS_MAX_INACTIVITY = 5 * 24 * 60 * 60 * 1000  # 5 days


def telegram_bot(args):
    _check_operations_json(args)
    _check_core_file(args)
    _check_app_running(args)

    return True


def _check_operations_json(args):
    with open(args.path_to_operations, "r", encoding="utf-8") as f:
        content = f.read()
        operations = json.loads(f"[{content}]")

    now = round(time.time() * 1000)
    last_timestamp = operations[-1]["timestamp"]

    delta = now - last_timestamp

    if delta > OPERATIONS_MAX_INACTIVITY:
        _send_message(args, msg_operations_inactivity)


def _check_core_file(args):
    pass


def _check_app_running(args):
    pass


def _send_message(args, msg):
    logger.info(f"Send message: {msg}")

    requests.post(TELEGRAM_SEND_URL, json={"chat_id": args.chat_id, "text": msg})


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--chat-id",
        dest="chat_id",
        type=str,
        default="",
        help="Telegram chat ID"
    )
    parser.add_argument(
        "--path-to-operations",
        dest="path_to_operations",
        type=str,
        default="",
        help="Path to operations.json file"
    )
    args = parser.parse_args()

    if args.chat_id == "":
        logger.error("Please specify Telegram chat ID with --chat-id")

        sys.exit(1)

    if args.path_to_operations == "":
        logger.error("Please specify path to operations.json file with --path-to-operations")

        sys.exit(1)

    sys.exit(0 if telegram_bot(args) else 1)
