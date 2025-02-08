from pathlib import Path

from checks.utils import run_checkers

group_name = "cpp"
files_pattern = ["*.h", "*.cpp"]

check_modules = [
    "clang_format",
]


def run_cpp_checkers(args):
    paths = []
    for pattern in files_pattern:
        paths.extend(Path(".").rglob(pattern))

    matched_files = sorted([str(path.absolute()) for path in paths if path.is_file()])

    return run_checkers(args, group_name, matched_files, check_modules)
