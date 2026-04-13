import os
import requests

from loguru import logger


TELEGRAM_TOKEN = os.environ["TELEGRAM_TOKEN"]
TELEGRAM_CHAT_ID = os.environ["TELEGRAM_CHAT_ID"]
TELEGRAM_SEND_URL = f"https://api.telegram.org/bot{TELEGRAM_TOKEN}/sendMessage"


def send_message(msg):
    logger.info(f"Send message: {msg}")

    requests.post(TELEGRAM_SEND_URL, json={"chat_id": TELEGRAM_CHAT_ID, "text": msg})
