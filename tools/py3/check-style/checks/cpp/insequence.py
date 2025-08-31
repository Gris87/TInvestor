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
        if "EXPECT_CALL(" in line:
            index = i - 1
            found = False

            while (index >= 0):
                prev_line = lines[index].rstrip()

                if "InSequence seq;" in prev_line:
                    found = True

                    break

                if prev_line == "{":
                    break

                index = index - 1

            if not found:
                logger.error(f"{file_path}:{i + 1}: InSequence seq should be provided before EXPECT_CALL()")

                res = False

    for i, line in enumerate(lines):
        if "InSequence seq;" in line:
            index = i + 1
            found = False

            while (index < len(lines)):
                next_line = lines[index].rstrip()

                if "EXPECT_CALL(" in next_line:
                    found = True

                    break

                if next_line == "}" or "InSequence seq;" in next_line:
                    break

                index = index + 1

            if not found:
                logger.error(f"{file_path}:{i + 1}: InSequence seq provided without EXPECT_CALL()")

                res = False

    return res


def is_file_skipped(file_path):
    file_path = file_path.replace("\\", "/")

    skip_files = (
        "/.git/" in file_path or
        "/build/" in file_path or
        "/test_" not in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
