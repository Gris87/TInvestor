import os
import sys
from loguru import logger

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from utils import prepare_linter


def do_validation(args):
    res = True

    for file_path in args.files:
        try:
            with open(file_path, "r") as f:
                lines = f.readlines()
        except Exception as e:
            continue

        res &= _validate_file(args, file_path, lines)

    return res


def _validate_file(args, file_path, lines):
    res = True

    for i, line in enumerate(lines):
        if line.startswith("#include \"") and not line.startswith("#include \"src/") and not line.startswith("#include \"ui_"):
            logger.error(f"{file_path}:{i + 1}: include should start with src/")

            res = False

    return res


def is_file_skipped(file_path):
    skip_files = (
        "\\.git\\" in file_path or
        "\\build\\" in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
