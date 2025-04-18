from pathlib import Path

from checks.utils import run_checkers

group_name = "clang_tidy"

check_modules = [
    "clang_tidy",
]


def run_clang_tidy_checkers(args):
    matched_files = [str(Path(".clang-tidy").absolute())]

    return run_checkers(args, group_name, matched_files, check_modules)
