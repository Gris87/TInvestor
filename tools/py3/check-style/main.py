import argparse
import os
import subprocess
import sys
from colorama import just_fix_windows_console
from concurrent.futures import ThreadPoolExecutor

from checks.common_checks import run_common_checkers
from checks.cpp_checks import run_cpp_checkers


check_groups = [
    run_common_checkers,
    run_cpp_checkers,
]


def check_style(args):
    just_fix_windows_console()

    commands = []

    for check_group in check_groups:
        commands.extend(check_group(args))

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

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(os.device_encoding(1)))

    process.wait()

    return process.returncode == 0, lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--fix",
        default=False,
        action="store_true",
        help="Fix issues",
    )
    args = parser.parse_args()

    if check_style(args):
        print("SUCCESS")

        sys.exit(0)

    print("FAILURE")

    sys.exit(1)
