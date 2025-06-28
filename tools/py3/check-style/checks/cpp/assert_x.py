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
        line = line.strip()

        if line.startswith("Q_ASSERT_X("):
            index = line.find('");')
            j = i + 1

            while index < 0 and j < len(lines):
                line += lines[j].strip()
                index = line.find('");')
                j += 1

            if '__FUNCTION__' not in line:
                logger.error(f'{file_path}:{i + 1}: Expecting for use of `__FUNCTION__` in Q_ASSERT_X()')

                res = False

    return res


def is_file_skipped(file_path):
    file_path = file_path.replace("\\", "/")

    skip_files = (
        "/.git/" in file_path or
        "/build/" in file_path or
        "/libs/qxlsx/src/qxlsx/" in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
