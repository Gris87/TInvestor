import argparse
import os
import requests
import sys
import time

from datetime import datetime
from loguru import logger

from localization import *


TELEGRAM_TOKEN = os.environ["TELEGRAM_TOKEN"]
TELEGRAM_SEND_URL = f"https://api.telegram.org/bot{TELEGRAM_TOKEN}/sendMessage"

MS_IN_SECOND           = 1000
ONE_MINUTE             = 60 * MS_IN_SECOND
LAST_MESSAGES_INTERVAL = 5 * ONE_MINUTE  # 5 minutes


def process_stock(args):
    posts = _get_pulse_posts(args)
    _process_posts(args, posts)

    return True


def _get_pulse_posts(args):
    url = f"https://api-invest.tbank.ru/invest-terminal/api-invest-gw/social/post/feed/v1/post/instrument/{args.ticker}?limit=20&include=all"
    resp = requests.get(url)

    resp_json = resp.json()

    return resp_json["payload"]["items"]


def _process_posts(args, posts):
    limit_timestamp = int(time.time() * 1000) - LAST_MESSAGES_INTERVAL

    for post in posts:
        post_inserted = int(datetime.fromisoformat(post["inserted"]).timestamp() * 1000)

        if post_inserted < limit_timestamp:
            break

        post_instruments = post["content"]["instruments"]

        if len(post_instruments) != 1:
            continue

        post_text = post["content"]["text"]
        post_text_simplified = post_text.casefold()

        if dividends.casefold() in post_text_simplified and recommend.casefold() in post_text_simplified:
            _send_message(args, post_text)


def _send_message(args, msg):
    logger.info(f"Send message: {msg}")

    requests.post(TELEGRAM_SEND_URL, json={"chat_id": args.chat_id, "text": msg})


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--chat-id",
        dest="chat_id",
        type=str,
        default="",
        help="Telegram chat ID"
    )
    parser.add_argument(
        "--ticker",
        dest="ticker",
        type=str,
        default="",
        help="Instrument ticker",
    )
    args = parser.parse_args()

    sys.exit(0 if process_stock(args) else 1)
