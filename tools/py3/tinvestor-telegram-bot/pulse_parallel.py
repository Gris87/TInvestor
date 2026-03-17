import argparse
import requests
import sys
import time

from datetime import datetime
from http import HTTPStatus
from loguru import logger

from localization import *
from messaging import send_message


PULSE_URL = "https://api-invest.tbank.ru/invest-terminal/api-invest-gw/social/post/feed/v1/post/instrument/{ticker}?limit=20&include=all"

MS_IN_SECOND           = 1000
ONE_MINUTE             = 60 * MS_IN_SECOND
LAST_MESSAGES_INTERVAL = 5 * ONE_MINUTE  # 5 minutes


def process_stock(args):
    posts = _get_pulse_posts(args)
    _process_posts(args, posts)

    return True


def _get_pulse_posts(args):
    while True:
        resp = requests.get(PULSE_URL.format(ticker=args.ticker))

        if resp.status_code != HTTPStatus.OK:
            time.sleep(1)

            continue

        resp_json = resp.json()

        return resp_json["payload"]["items"]


def _process_posts(args, posts):
    limit_timestamp = round(time.time() * MS_IN_SECOND) - LAST_MESSAGES_INTERVAL

    for post in posts:
        post_inserted = round(datetime.fromisoformat(post["inserted"]).timestamp() * MS_IN_SECOND)

        if post_inserted < limit_timestamp:
            break

        post_instruments = post["content"]["instruments"]

        if len(post_instruments) != 1:
            continue

        post_text = post["content"]["text"]
        post_text_simplified = post_text.casefold()

        match = recommend_to_buy_regexp.match(post_text_simplified)

        if match is not None:
            found_text = match.group(1).strip()

            send_message(args.chat_id, msg_recommend_to_buy + "\n" + msg_pulse_text_found.format(found_text=found_text) + "\n" + post_text)

        match = recommend_to_short_regexp.match(post_text_simplified)

        if match is not None:
            found_text = match.group(1).strip()

            send_message(args.chat_id, msg_recommend_to_short + "\n" + msg_pulse_text_found.format(found_text=found_text) + "\n" + post_text)


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
