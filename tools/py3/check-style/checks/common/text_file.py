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
            with open(file_path, "rb") as f:
                content = f.read()

            if _is_text_file(content):
                try:
                    lines = content.decode("utf-8").split("\n")
                except UnicodeDecodeError:
                    continue

                res &= _validate_file(args, file_path, content, lines)
        except Exception as e:
            pass

    return res


def _validate_file(args, file_path, content, lines):
    res = True

    if args.fix:
        lines = _fix_file(lines)

        new_content = "\n".join(lines).encode("utf-8")

        if new_content != content:
            with open(file_path, "wb") as f:
                f.write(new_content)
    else:
        res &= _check_for_lf(file_path, content)
        res &= _check_for_newline_at_bof(file_path, lines)
        res &= _check_for_newline_at_eof(file_path, lines)
        res &= _check_for_multiple_newlines(file_path, lines)
        res &= _check_for_trailing_whitespaces(file_path, lines)
        res &= _check_for_mixing_whitespaces(file_path, lines)
        res &= _check_for_non_ascii(file_path, lines)

    return res


def _check_for_lf(file_path, content):
    if b"\r" in content:
        logger.error(f"{file_path}: Unix style line endings should be used")
        return False

    return True


def _check_for_newline_at_bof(file_path, lines):
    if len(lines) > 1 and (lines[0] == "" or lines[0] == "\r"):
        logger.error(f"{file_path}: Newline found at the beginning of file")

        return False

    return True


def _check_for_newline_at_eof(file_path, lines):
    last_not_empty_line = 0

    for i, line in reversed(list(enumerate(lines))):
        if line != "" and line != "\r":
            last_not_empty_line = i
            break

    if last_not_empty_line == len(lines) - 1:
        if last_not_empty_line != 0 or lines[0] != "":
            logger.error(f"{file_path}: Newline not found at the end of file")
            return False
    elif last_not_empty_line != len(lines) - 2:
        logger.error(f"{file_path}: Multiple newlines found at the end of file")
        return False

    return True


def _check_for_multiple_newlines(file_path, lines):
    res = True

    allowed_newlines = [0, 1, 3]

    if file_path.endswith(".py"):
        allowed_newlines = [0, 1, 2]

    amount_of_newlines = 0

    for i, line in enumerate(lines):
        if line == "" or line == "\r":
            amount_of_newlines = amount_of_newlines + 1
        else:
            if amount_of_newlines not in allowed_newlines:
                logger.error(f"{file_path}:{i + 1}: Multiple newlines found")

                res = False

            amount_of_newlines = 0

    return res


def _check_for_trailing_whitespaces(file_path, lines):
    res = True

    for i, line in enumerate(lines):
        positions = []
        for j, ch in enumerate(line[::-1]):
            if ch not in [" ", "\t", "\r"]:
                break
            if ch != "\r":
                positions.append(j)
        if len(positions) != 0:
            logger.error(f"{file_path}:{i + 1}: Trailing whitespaces found")
            res = False

    return res


def _check_for_mixing_whitespaces(file_path, lines):
    res = True

    for i, line in enumerate(lines):
        prev_ch = "_"
        mixing = False
        for j, ch in enumerate(line):
            if prev_ch == " " and ch == "\t":
                mixing = True
                break
            prev_ch = ch
        if mixing:
            logger.error(f"{file_path}:{i + 1}: Mixing whitespaces found")

            res = False

    return res


def _check_for_non_ascii(file_path, lines):
    res = True

    file_path_simplified = file_path.replace("\\", "/")

    skip_files = (
        "/app/assets/translations/language_ru.ts" in file_path_simplified
    )

    if skip_files:
        return res

    for i, line in enumerate(lines):
        positions = []
        line = line.replace("\t", "    ")

        for j, ch in enumerate(line):
            code = ord(ch)
            if (code < 0x20 or code > 0x7E) and ch != "\r":
                positions.append(j)

        if len(positions) != 0:
            position_markers = [" "] * (positions[-1] + 1)

            for position in positions:
                position_markers[position] = "^"

            logger.error(f"{file_path}:{i + 1}: Non-ASCII characters found")
            logger.info(line)
            logger.info("".join(position_markers))

            res = False

    return res


def _fix_file(lines):
    new_lines = []

    skip_newlines_at_bof = True

    for i, line in enumerate(lines):
        line = _fix_line(line)

        if skip_newlines_at_bof:
            if line == "":
                continue

            skip_newlines_at_bof = False

        new_lines.append(line)

    while len(new_lines) > 0 and new_lines[-1] == "":
        del new_lines[-1]

    new_lines.append("")

    return new_lines


def _fix_line(line):
    line = line.rstrip()

    new_line = []

    skip_tabs_at_bol = True
    index_for_tab = -1

    for i, ch in enumerate(line):
        if skip_tabs_at_bol:
            if ch != "\t":
                skip_tabs_at_bol = False
                index_for_tab = i
        else:
            if ch == "\t":
                new_line.append(" " * (4 - (i - index_for_tab) % 4))
                index_for_tab = i + 1
                continue

        new_line.append(ch)

    return "".join(new_line)


def _is_text_file(content):
    textchars = bytearray(
        {7, 8, 9, 10, 12, 13, 27} | set(range(0x20, 0x100)) - {0x7F}
    )

    return not bool(content.translate(None, textchars))


def is_file_skipped(file_path):
    file_path = file_path.replace("\\", "/")

    skip_files = (
        "/.git/" in file_path or
        "/build/" in file_path or
        "/libs/investapi/messages/" in file_path or
        "/libs/simdjson/src/simdjson/" in file_path or
        "/tools/py3/tinkoff-accounts/tinkoff/" in file_path or
        "/tools/py3/tinkoff-follower/tinkoff/" in file_path or
        "/TInvestor.pro.user" in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
