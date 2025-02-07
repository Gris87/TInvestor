import os
import sys
from loguru import logger

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from utils import prepare_linter


def do_validation(args):
    res = True

    for file_path in args.files:
        res &= _validate_file(file_path)

    return res


def _validate_file(file_path):
    logger.error(f"{file_path}: Bad file found")

    return False


def is_file_skipped(file_path):
    skip_files = (
        "\\tools\\py3\\check-style\\checks\\common\\" in file_path
    )

    if skip_files:
        return True

    return "mrgtmp" not in file_path


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
