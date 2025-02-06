from pathlib import Path

from checks.utils import run_checkers

group_name = "common"
files_pattern = "*"

check_modules = [
    "ascii",
]


def run_common_checkers():
    matched_files = sorted([str(path.absolute()) for path in Path(".").rglob(files_pattern) if path.is_file()])

    return run_checkers(group_name, matched_files, check_modules)
