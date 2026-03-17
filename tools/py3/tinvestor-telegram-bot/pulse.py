import json
import os
import subprocess

from concurrent.futures import ThreadPoolExecutor
from loguru import logger
from pathlib import Path


PATH_TO_SCRIPT = Path(__file__).parent


def check_pulse(args):
    with open(Path(args.path_to_stocks) / "stocks.json", "r", encoding="utf-8") as f:
        content = f.read()
        stocks_meta = json.loads(content)

    commands = []

    for stock_meta in stocks_meta:
        commands.append(
            [
                "python",
                str(Path(PATH_TO_SCRIPT) / "pulse_parallel.py"),
                "--ticker", stock_meta["instrumentTicker"]
            ]
        )

    return _execute_commands(commands)


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, lines in executor.map(_execute_command, commands):
            res &= result

            for line in lines:
                print(line)

    return res


def _execute_command(command):
    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )

    lines = []

    encoding = os.device_encoding(1)
    if encoding is None:
        encoding = "utf-8"

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return process.returncode == 0, lines
