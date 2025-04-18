import os
import subprocess
import sys
from loguru import logger

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from utils import prepare_linter


def do_validation(args):
    res = True

    for file_path in args.files:
        res &= _validate_file(args, file_path)

    return res


def _validate_file(args, file_path):
    if args.fix:
        argv = ["clang-format", "-i", f"{file_path}"]
    else:
        argv = ["clang-format", "--dry-run", "--Werror", f"{file_path}"]

    rc = subprocess.run(argv, capture_output=True)
    if rc.returncode != 0:
        logger.error(rc.stderr.decode("utf-8"))

        return False

    return True


def is_file_skipped(file_path):
    file_path = file_path.replace("\\", "/")

    skip_files = (
        "/.git/" in file_path or
        "/build/" in file_path or
        "/libs/investapi/messages/" in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
