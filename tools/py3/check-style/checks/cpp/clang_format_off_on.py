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
    clang_format_enabled = True
    last_off_line = -1

    for i, line in enumerate(lines):
        line = line.strip()

        if line == "// clang-format off":
            if clang_format_enabled:
                clang_format_enabled = False
                last_off_line = i
            else:
                logger.error(f"{file_path}:{last_off_line + 1}: clang-format was not enabled")

                return False
        elif line == "// clang-format on":
            if not clang_format_enabled:
                clang_format_enabled = True
            else:
                logger.error(f"{file_path}:{i + 1}: clang-format is enabled already")

                return False

    if not clang_format_enabled:
        logger.error(f"{file_path}:{last_off_line + 1}: clang-format was not enabled")

        return False

    return True


def is_file_skipped(file_path):
    skip_files = (
        "\\.git\\" in file_path or
        "\\build\\" in file_path or
        "\\libs\\investapi\\messages\\" in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
