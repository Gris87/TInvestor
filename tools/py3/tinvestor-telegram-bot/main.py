import argparse
import json
import os
import sys

from loguru import logger
from pathlib import Path


def telegram_bot(args):
    cache_folder_path = Path(args.cache) / "telegram"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    state_path = cache_folder_path / "state.json"

    last_timestamp = 0

    if state_path.exists():
        with open(state_path, "r", encoding="utf-8") as f:
            old_state = json.loads(f.read())

        last_timestamp = old_state["last_timestamp"]

    notifications_path = Path(args.path_to_notifications)

    for record_path in sorted(notifications_path.rglob("*.json"), key=lambda x: x.stat().st_mtime):
        if record_path.stat().st_mtime >= last_timestamp:
            last_timestamp = record_path.stat().st_mtime

            _process_file(record_path)

    state = {
        "last_timestamp": last_timestamp + 1
    }

    with open(state_path, "w", encoding="utf-8") as f:
        json.dump(state, f, ensure_ascii=False)

    return True


def _process_file(record_path):
    with open(record_path, "r", encoding="utf-8") as f:
        record = json.loads(f.read())

    _send_message(record["text"])


def _send_message(msg):
    logger.info(f"Send message: {msg}")


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
    args = parser.parse_args()

    expected_env_vars = [
        "TELEGRAM_TOKEN",
        "TELEGRAM_CHAT_ID"
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

    sys.exit(0 if telegram_bot(args) else 1)
