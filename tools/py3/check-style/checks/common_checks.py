from pathlib import Path

from checks.utils import run_checkers

group_name = "common"
files_pattern = ["*"]

check_modules = [
    "mrgtmp",
    "text_file",
]


def run_common_checkers(args):
    paths = []
    for pattern in files_pattern:
        paths.extend(Path(".").rglob(pattern))

    matched_files = sorted([str(path.absolute()) for path in paths if path.is_file()])

    return run_checkers(args, group_name, matched_files, check_modules)
