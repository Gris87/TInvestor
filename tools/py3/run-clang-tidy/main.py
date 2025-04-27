import argparse
import math
import os
import re
import subprocess
import sys
from colorama import just_fix_windows_console, Fore, Style
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


files_pattern = ["*.h", "*.cpp"]
extra_args = [
    "-p",
    Path(Path(os.getcwd()) / "build/Desktop-Debug/.qtc_clangd").absolute(),
]

ignored_output = [
    re.compile(r"\d+ warnings generated\."),
    re.compile(r"Suppressed \d+ warnings \(.*\)\."),
    re.compile(r"Use -header-filter=\.\* to display errors from all non-system headers\. Use -system-headers to display errors from system headers as well\."),
]


def run_clang_tidy(args):
    just_fix_windows_console()

    paths = []
    for pattern in files_pattern:
        paths.extend(Path("app").rglob(pattern))

    matched_files = sorted([str(path.absolute()) for path in paths if path.is_file() and not path.name.startswith("test_")])

    number_of_parts = min(max(args.number_of_parts, 1), len(matched_files))
    part = min(max(args.part, 1), number_of_parts)

    chunk_size = math.ceil(len(matched_files) / number_of_parts);
    file_chunks = [matched_files[i : i + chunk_size] for i in range(0, len(matched_files), chunk_size)]
    chunk = file_chunks[part - 1]

    print(f"Files in chunk: {len(chunk)}")

    commands = []

    for file_path in chunk:
        command = ["clang-tidy"]
        command.extend(extra_args)
        command.append(file_path)

        commands.append(command)

    return _execute_commands(commands)


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for file_path, result, lines in executor.map(_execute_command, commands):
            print(f"Analyzed {file_path}")

            if result:
                for line in lines:
                    print(line)
            else:
                res = False

                for line in lines:
                    print(Fore.RED + line + Style.RESET_ALL)

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
        decoded_line = line.rstrip().decode(encoding)

        good = True

        for pattern in ignored_output:
            match = pattern.match(decoded_line)

            if match is not None:
                good = False

                break

        if good:
            lines.append(decoded_line)

    process.wait()

    return command[-1], process.returncode == 0, lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--part",
        dest="part",
        type=int,
        default=1,
        help="Part index"
    )
    parser.add_argument(
        "--number-of-parts",
        dest="number_of_parts",
        type=int,
        default=1,
        help="Amount of parts"
    )
    args = parser.parse_args()

    if run_clang_tidy(args):
        print("SUCCESS")

        sys.exit(0)

    print("FAILURE")

    sys.exit(1)
