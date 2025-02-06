import argparse
import importlib
import traceback
from pathlib import Path

path_to_script = Path(__file__).parent

max_files_for_linter = 100


def run_checkers(group_name, matched_files, check_modules):
    res = []

    if not matched_files:
        return res

    for module in check_modules:
        python_module = importlib.import_module(f"checks.{group_name}.{module}")
        files = [file_path for file_path in matched_files if not python_module.is_file_skipped(file_path)]

        res.extend(validate_files(f"{group_name}/{module}.py", files))

    return res


def validate_files(module, files):
    res = []

    file_chunks = [files[i : i + max_files_for_linter] for i in range(0, len(files), max_files_for_linter)]

    for chunk in file_chunks:
        file_args = " ".join([f"--file {file}" for file in chunk])

        res.append(
            f"python3 {path_to_script}/{module} {file_args}"
        )

    return res


def prepare_linter():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--file",
        dest="files",
        type=str,
        action="append",
        help="Path to file",
    )
    args = parser.parse_args()

    return args
