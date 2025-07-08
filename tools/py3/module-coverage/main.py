import os
import re
import shutil
import subprocess
import sys
import xml.etree.ElementTree as ET
from concurrent.futures import ThreadPoolExecutor
from loguru import logger
from pathlib import Path


test_regexp = re.compile(r'TEST(?:_F)?\((\w+), *\w+\)')


def module_coverage():
    shutil.rmtree("build/ModuleCoverage", ignore_errors=True)

    output_path = Path(f"build/ModuleCoverage/results")
    output_path.mkdir(parents=True, exist_ok=True)

    files = _get_files()
    commands = _get_commands(files)

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
            "--export_type",
            f"cobertura:..\\..\\..\\ModuleCoverage\\{module_path_simplified}\\cobertura.xml",
            "--",
            "tests.exe",
            f"--gtest_filter={test_group}.*"
        ]

        res.append({
            "cmd": cmd,
            "module_path": module_path,
            "module_path_simplified": module_path_simplified,
        })

    return res


def _execute_commands(commands):
    res = True

    non_full_modules = []

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for result, module_path, coverage in executor.map(_execute_command, commands):
            res &= result

            if coverage == 100:
                print(f"{module_path:120}: {coverage:5.2f}%")
            else:
                non_full_modules.append({
                    "module_path": module_path,
                    "coverage": coverage,
                })

    if non_full_modules:
        non_full_modules.sort(key=lambda x: x["coverage"], reverse=True)

        for module in non_full_modules:
            module_path = module["module_path"]
            coverage = module["coverage"]

            print(f"{module_path:120}: {coverage:5.2f}%")

        logger.error(f'Some modules not fully covered')

    return res


def _execute_command(command):
    cmd = command["cmd"]
    module_path = command["module_path"]
    module_path_simplified = command["module_path_simplified"]

    for i in range(5):
        shutil.rmtree(f"build/ModuleCoverage/{module_path_simplified}", ignore_errors=True)

        process = subprocess.Popen(
            cmd,
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

        if process.returncode != 0:
            continue

        tree = ET.parse(f"build/ModuleCoverage/{module_path_simplified}/cobertura.xml")
        root = tree.getroot()

        return True, module_path, float(root.get("line-rate", "0")) * 100

    return False, module_path, 0


def main():
    sys.exit(0 if module_coverage() else 1)
