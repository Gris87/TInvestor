import os
import re
import sys
from loguru import logger

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from utils import prepare_linter


disabled_linter_regexp = re.compile(r"# ([^ ]+) +\-.*")


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

    disabled_linters_desc = []
    disabled_linters      = []

    for i, line in enumerate(lines):
        line = line.strip()
        match = disabled_linter_regexp.match(line)

        if match is not None:
            disabled_linter = match.group(1)
            disabled_linters_desc.append(disabled_linter)

        if line == "*,":
            i += 1

            while i < len(lines):
                line = lines[i].strip()

                if line == "'":
                    break

                disabled_linters.append(line.replace(",", "").strip()[1:])

                i += 1

            break

    sorted_disabled_linters_desc = sorted(disabled_linters_desc)

    if disabled_linters_desc != sorted_disabled_linters_desc:
        logger.error(f"{file_path}: Disabled linters is not sorted")

        res = False

    if disabled_linters_desc != disabled_linters:
        linters_without_desc = [x for x in disabled_linters if x not in disabled_linters_desc]
        non_disabled_linters = [x for x in disabled_linters_desc if x not in disabled_linters]


        if len(linters_without_desc) > 0:
            linters_without_desc_str = ", ".join(linters_without_desc)

            logger.error(f"{file_path}: Linters without description: [{linters_without_desc_str}]")

            res = False

        if len(non_disabled_linters) > 0:
            non_disabled_linters_str = ", ".join(non_disabled_linters)

            logger.error(f"{file_path}: Non disabled linters: [{non_disabled_linters_str}]")

            res = False

    return res


def is_file_skipped(file_path):
    return False


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
