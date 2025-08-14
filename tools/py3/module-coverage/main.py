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

is_windows = os.name == 'nt'


def module_coverage():
    shutil.rmtree("build/ModuleCoverage", ignore_errors=True)

    output_path = Path(f"build/ModuleCoverage/results")
    output_path.mkdir(parents=True, exist_ok=True)

    files = _get_files()
    commands = _get_commands(files)

    print(f"Found {len(files)} files")

    return _execute_commands(commands)


def _get_files():
    res = []

    for path in sorted(Path("app").rglob("*.cpp")):
        if path.name.startswith("test_"):
            continue

        file_path = str(path.absolute()).replace("\\", "/")

        skip_files = (
            "app/src/main.cpp" in file_path or
            "app/src/utils/filedialog/filedialog.cp" in file_path or
            "app/src/utils/http/httpclient.cpp" in file_path or
            "app/src/utils/messagebox/messageboxutils.cp" in file_path
        )

        if skip_files:
            continue

        if not Path(path.parent / f"test_{path.name}").exists():
            logger.error(f'{file_path}: Test file not found')
            sys.exit(1)

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

        if is_windows:
            prepare_cmd = []
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
            post_cmd = []
        else:
            prepare_cmd = [
                "./tests",
                f"--gtest_filter={test_group}.*"
            ]
            cmd = [
                "lcov",
                "-t",
                "tests",
                "--capture",
                "--include",
                module_path,
                "--directory",
                "gen/tests/objs",
                "--output-file",
                "lcov.info"
            ]
            post_cmd = [
                "genhtml",
                "--output-directory",
                f"../../../ModuleCoverage/{module_path_simplified}",
                "lcov.info",
            ]

        res.append({
            "prepare_cmd": prepare_cmd,
            "cmd": cmd,
            "post_cmd": post_cmd,
            "module_path": module_path,
            "module_path_simplified": module_path_simplified,
        })

    return res


def _execute_commands(commands):
    res = True

    if is_windows:
        number_of_workers = os.cpu_count()
    else:
        number_of_workers = 1

    cur = 0

    non_full_modules = []

    with ThreadPoolExecutor(number_of_workers) as executor:
        for result, module_path, coverage in executor.map(_execute_command, commands):
            res &= result

            if coverage == 100:
                cur += 1

                print(f"{cur:4} {module_path:120}: {coverage:5.2f}%")
            else:
                non_full_modules.append({
                    "module_path": module_path,
                    "coverage": coverage,
                })

    if non_full_modules:
        non_full_modules.sort(key=lambda x: x["coverage"], reverse=True)

        for module in non_full_modules:
            cur += 1

            module_path = module["module_path"]
            coverage = module["coverage"]

            print(f"{cur:4} {module_path:120}: {coverage:5.2f}%")

        logger.error(f'Some modules not fully covered')

    return res


def _execute_command(command):
    prepare_cmd = command["prepare_cmd"]
    cmd = command["cmd"]
    post_cmd = command["post_cmd"]
    module_path = command["module_path"]
    module_path_simplified = command["module_path_simplified"]

    my_env = os.environ.copy()
    my_env["LD_LIBRARY_PATH"] = str(Path("build/Desktop-Debug/test/build").absolute())

    for i in range(5):
        shutil.rmtree(f"build/ModuleCoverage/{module_path_simplified}", ignore_errors=True)

        if not is_windows:
            process = subprocess.Popen(
                prepare_cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                cwd="build/Desktop-Debug/test/build",
                env=my_env
            )

            process.wait()

            if process.returncode != 0:
                continue

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

        if not is_windows:
            process = subprocess.Popen(
                post_cmd,
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                cwd="build/Desktop-Debug/test/build"
            )

            process.wait()

            if process.returncode != 0:
                continue

        if is_windows:
            tree = ET.parse(f"build/ModuleCoverage/{module_path_simplified}/cobertura.xml")
            root = tree.getroot()

            coverage = float(root.get("line-rate", "0")) * 100
        else:
            total_lines = 0
            covered_lines = 0

            with open(f"build/Desktop-Debug/test/build/lcov.info", "r") as f:
                lines = f.readlines()

            for line in lines:
                line = line.strip()

                if line.startswith("DA"):
                    total_lines += 1

                    if not line.endswith(",0"):
                        covered_lines += 1

            coverage = (covered_lines / total_lines) * 100.0 if total_lines > 0 else 100.0

        output_path = Path(f"build/ModuleCoverage/results/{int(coverage)}")
        output_path.mkdir(parents=True, exist_ok=True)

        shutil.move(f"build/ModuleCoverage/{module_path_simplified}", f"build/ModuleCoverage/results/{int(coverage)}/{module_path_simplified}")

        if not is_windows:
            shutil.move("build/Desktop-Debug/test/build/lcov.info", f"build/ModuleCoverage/results/{int(coverage)}/{module_path_simplified}/lcov.info")

        return True, module_path, coverage

    return False, module_path, 0


def main():
    sys.exit(0 if module_coverage() else 1)
