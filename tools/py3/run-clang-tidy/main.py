import os
import subprocess
import sys
from colorama import just_fix_windows_console, Fore, Style
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


files_pattern = ["*.h", "*.cpp"]
extra_args = ["-p", Path(Path(os.getcwd()) / "build/Desktop-Debug/.qtc_clangd").absolute()]


def run_clang_tidy():
    just_fix_windows_console()

    paths = []
    for pattern in files_pattern:
        paths.extend(Path("app").rglob(pattern))
        paths.extend(Path("test").rglob(pattern))

    matched_files = sorted([str(path.absolute()) for path in paths if path.is_file()])

    commands = []

    for file_path in matched_files:
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
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return command[-1], process.returncode == 0, lines


def main():
    if run_clang_tidy():
        print("SUCCESS")

        sys.exit(0)

    print("FAILURE")

    sys.exit(1)
