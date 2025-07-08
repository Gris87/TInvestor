import os
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from loguru import logger
from pathlib import Path


test_regexp = re.compile(r'TEST(?:_F)?\((\w+), *\w+\)')


def module_coverage():
    shutil.rmtree("build/ModuleCoverage", ignore_errors=True)

    output_path = Path(f"build/ModuleCoverage")
    output_path.mkdir(parents=True, exist_ok=True)

    files = _get_files()
    commands = _get_commands(files)

    commands = commands[:4] # TODO: Remove

    return _execute_commands(commands)


def _get_files():
    res = []

    for path in sorted(Path("app").rglob("*.cpp")):
        if path.name.startswith("test_"):
            continue

        if not Path(path.parent / f"test_{path.name}").exists():
            file_path = str(path.absolute()).replace("\\", "/")

            if "app/src/main.cpp" not in file_path:
                logger.error(f'{file_path}: Test file not found')

            continue

        res.append(path)

    return res


def _get_commands(files):
    res = []

    for path in files:
        module_path = str(path)
        module_path_simplified = module_path.replace("\\", "_").replace("/", "_").replace(".cpp", "")

        test_path = Path(path.parent / f"test_{path.name}")

        with open(test_path, "r") as f:
            lines = f.readlines()

        test_group = ""

        for line in lines:
            match = test_regexp.match(line)

            if match is not None:
                test_group = match.group(1)

                break

        cmd = [
            "OpenCppCoverage",
            "--sources",
            module_path,
            "--export_type",
            f"html:..\\..\\..\\ModuleCoverage\\{module_path_simplified}",
            "--",
            "tests.exe",
            f"--gtest_filter={test_group}.*"
        ]

        res.append({
            "cmd": cmd,
            "module_path_simplified": module_path_simplified,
        })

    return res


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, lines in executor.map(_execute_command, commands):
            res &= result

            if not result:
                for line in lines:
                    print(line)

    return res


def _execute_command(command):
    process = subprocess.Popen(
        command["cmd"],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd="build/Desktop-Debug/test/build"
    )

    lines = []

    encoding = os.device_encoding(1)
    if encoding is None:
        encoding = "utf-8"

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return process.returncode == 0, lines


def main():
    sys.exit(0 if module_coverage() else 1)
