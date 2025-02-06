import os
import shlex
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

from checks.common_checks import run_common_checkers


check_groups = [
    run_common_checkers,
]


def check_style():
    commands = []

    for check_group in check_groups:
        commands.extend(check_group())

    return _execute_commands(commands)


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, bytes in executor.map(_execute_command, commands):
            res &= result

            sys.stdout.buffer.write(bytes)
            sys.stdout.buffer.flush()

    return res


def _execute_command(command):
    argv = shlex.split(command, posix=False)

    process = subprocess.Popen(
        argv,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
    )

    process.wait()

    return process.returncode == 0, process.stdout.read() + process.stderr.read()


def main():
    if check_style():
        print("SUCCESS")

        sys.exit(0)

    print("FAILURE")

    sys.exit(1)
