import json
import requests

from datetime import date
from dateutil.relativedelta import relativedelta
from http import HTTPStatus
from loguru import logger
from pathlib import Path

from localization import *
from messaging import send_message


DIVIDENDS_URL="https://api-invest.tbank.ru/invest-terminal/api-invest-gw/fireg-advisory/invest-calendar/v2/event/info?startDate={start_date}&endDate={end_date}&mySecuritiesFlag=false&qualFlag=false&eventTypes=dividends&limit=100"


def check_dividends(args):
    calendar = _get_dividend_calendar()
    dividends = _convert_calendar_to_dividends(calendar)

    cache_folder_path = Path(args.cache) / "dividends"
    cache_folder_path.mkdir(parents=True, exist_ok=True)

    dividends_path = cache_folder_path / "dividends.json"

    if dividends_path.exists():
        with open(dividends_path, "r", encoding="utf-8") as f:
            old_dividends = json.loads(f.read())

        for instrument_id, dividend in dividends.items():
            if instrument_id not in old_dividends:
                send_message(args.chat_id, msg_recommend_to_buy + "\n" + msg_dividends.format(ticker=dividend["ticker"], name=dividend["name"], yield_value=dividend["yieldValue"]))

    with open(dividends_path, "w", encoding="utf-8") as f:
        json.dump(dividends, f, ensure_ascii=False)

    return True


def _get_dividend_calendar():
    today = date.today()
    future = today + relativedelta(months=6)

    start_date = today.strftime("%Y-%m-%d")
    end_date = future.strftime("%Y-%m-%d")

    while True:
        resp = requests.get(DIVIDENDS_URL.format(start_date=start_date, end_date=end_date))

        if resp.status_code != HTTPStatus.OK:
            time.sleep(1)

            continue

        resp_json = resp.json()

        return resp_json["dates"]


def _convert_calendar_to_dividends(calendar):
    res = {}

    for event in calendar:
        date = event["date"]
        dividends = event["dividends"]

        for dividend in dividends:
            instrument_id = dividend["instrumentUid"]
            dividend["date"] = date

            res[instrument_id] = dividend

    return res
