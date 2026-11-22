import asyncio
import argparse
import datetime as dt
import json
import logging
import shutil
import sys

from datetime import datetime, timedelta, timezone
from loguru import logger
from pathlib import Path
from zoneinfo import ZoneInfo

from t_tech.invest.constants import INVEST_GRPC_API
from t_tech.invest.retrying.aio.client import AsyncRetryingClient
from t_tech.invest.retrying.settings import RetryClientSettings
from t_tech.invest.utils import quotation_to_decimal


#logging.basicConfig(level=logging.DEBUG)


MOSCOW_TZ = ZoneInfo("Europe/Moscow")

WORKDAY_START = dt.time(10, 0, tzinfo=MOSCOW_TZ)
WORKDAY_END   = dt.time(23, 50, tzinfo=MOSCOW_TZ)

TMON_UID = "498ec3ff-ef27-4729-9703-a5aac48d5789"


async def tmon_trade_stats(args):
    if args.collect:
        await _collect_stats(args)
    else:
        await _display_stats(args)


async def _collect_stats(args):
    logger.info("Connecting to server")

    token = _get_token(args.token, args.token_file)
    retry_settings = RetryClientSettings(use_retry=True, max_retry_attempt=10000)

    async with AsyncRetryingClient(token, settings=retry_settings, target=INVEST_GRPC_API) as client:
        await _start_streaming(args, client)


def _get_token(token, token_file):
    if token != "":
        return token

    with open(token_file, "r") as f:
        return f.read().strip()


async def _start_streaming(args, client):
    from_dt = datetime.now(timezone.utc)

    while True:
        if not _is_weekend_work_time():
            await _clear_cache(args)

            seconds = _seconds_to_next_morning() + 1

            logger.info(f"Sleep for {seconds}s because current time is not valid")
            await asyncio.sleep(seconds)

            continue

        last_trades = await client.market_data.get_last_trades(instrument_id=TMON_UID)
        from_dt = await _handle_last_trades(args, last_trades.trades, from_dt)

        await asyncio.sleep(10)


async def _handle_last_trades(args, trades, from_dt):
    if len(trades) == 0:
        return from_dt

    cache_folder_path = Path(args.cache) / "tmon_trade_stats"
    cache_folder_path.mkdir(parents=True, exist_ok=True)
    trades_path = cache_folder_path / "trades.json"

    trades.sort(key=lambda x: x.time)

    data = {}

    for trade in trades:
        if trade.time > from_dt:
            time_str = trade.time.astimezone(MOSCOW_TZ).strftime("%H:%M")

            if time_str not in data:
                data[time_str] = {}

            price = quotation_to_decimal(trade.price)
            price_str = f"{price:.2f}"

            if price_str not in data[time_str]:
                data[time_str][price_str] = 0

            data[time_str][price_str] += trade.quantity

    with open(trades_path, "a", encoding="utf-8") as f:
        for time_str, prices in data.items():
            entry = {
                "time": time_str,
                "trades": dict(sorted(prices.items()))
            }

            json.dump(entry, f, ensure_ascii=False)
            f.write(",\n")

            logger.info(f"Added data for {time_str}")

    return trades[-1].time


async def _display_stats(args):
    print("Time    Trades")

    prev_trades = {}

    while True:
        results = await _handle_stats(args, _load_stats(args))
        await _print_results(results, prev_trades)

        prev_trades = results["trades"]

        await asyncio.sleep(60)


def _load_stats(args):
    res = []

    cache_folder_path = Path(args.cache) / "tmon_trade_stats"
    trades_path = cache_folder_path / "trades.json"

    if trades_path.exists():
        with open(trades_path, "r", encoding="utf-8") as f:
            content = f.read()

        content = content.rstrip().rstrip(",")

        res = json.loads("[" + content + "]")

    return res


async def _handle_stats(args, stats):
    if len(stats) == 0:
        return {
            "time": "00:00",
            "trades": {},
        }

    trades = {}

    for stat in stats:
        if stat["time"] >= args.from_time:
            for price, quantity in stat["trades"].items():
                if price not in trades:
                    trades[price] = 0

                trades[price] += quantity

    return {
        "time": stats[-1]["time"],
        "trades": dict(sorted(trades.items())),
    }


async def _print_results(results, prev_trades):
    time_str = results["time"]

    print(f"{time_str}   ", end="")

    for price, quantity in results["trades"].items():
        print(f"{price} - {quantity:6} ", end="")

        if price in prev_trades:
            delta = quantity - prev_trades[price]
            delta_str = f"(+{delta})"

            print(f"{delta_str:9}    ", end="")

        else:
            print("             ", end="")

    print("")


async def _clear_cache(args):
    shutil.rmtree(Path(args.cache) / "tmon_trade_stats", ignore_errors=True)


def _is_weekend_work_time():
    d = datetime.now(MOSCOW_TZ)

    if d.isoweekday() < 6:
        return False

    t = d.timetz()

    return t >= WORKDAY_START and t < WORKDAY_END


def _seconds_to_next_morning():
    now = datetime.now(MOSCOW_TZ)

    target_morning = now.replace(hour=WORKDAY_START.hour, minute=WORKDAY_START.minute, second=0, microsecond=0)

    if now >= target_morning:
        target_morning += timedelta(days=1)

    return int((target_morning - now).total_seconds())


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--collect",
        dest="collect",
        default=False,
        action="store_true",
        help="Flag for collecting statistics",
    )
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
    parser.add_argument(
        "--from",
        dest="from_time",
        type=str,
        default="",
        help="From time in format HH:MM",
    )
    parser.add_argument(
        "--cache",
        dest="cache",
        type=str,
        default="",
        help="Path to cache folder"
    )
    args = parser.parse_args()

    if args.collect:
        if (args.token == "" and args.token_file == "") or (args.token != "" and args.token_file != ""):
            logger.error("Please specify token with --token or --token-file")

            sys.exit(1)

    if args.cache == "":
        logger.error("Please specify path to cache folder with --cache")

        sys.exit(1)

    asyncio.run(tmon_trade_stats(args))
