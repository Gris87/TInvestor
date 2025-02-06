import os
import pty
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
    master_fd, slave_fd = pty.openpty()
    process = subprocess.Popen(command, shell=True, stdout=slave_fd, stdin=slave_fd, stderr=slave_fd, close_fds=True)
    os.close(slave_fd)

    output = b""
    while True:
        try:
            data = os.read(master_fd, 1024)
        except OSError:
            break

        if not data:
            break

        output += data

    return_code = process.wait()
    os.close(master_fd)

    return return_code == 0, output


def main():
     sys.exit(0 if check_style() else 1)
