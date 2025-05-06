import os
import re
import sys
from enum import auto, IntEnum
from loguru import logger

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
sys.path.append(os.path.dirname(SCRIPT_DIR))

from utils import prepare_linter


major_include_regexp = re.compile(r'#include [<"](?:src\/(?:\w+\/)*)?(\w+)(?:\.h)?[">]')


class GroupType(IntEnum):
    UNSPECIFIED = auto()
    MAJOR = auto()
    SYSTEM = auto()
    PROJECT = auto()


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

    start_index = -1
    end_index = -1

    for i, line in enumerate(lines):
        if line.strip().startswith("#include"):
            if start_index < 0:
                start_index = i

            end_index = i + 1

    if start_index < 0:
        return res

    import_lines = []

    for i in range(start_index, end_index):
        line = lines[i].strip()

        if not line.startswith("#if") and not line.startswith("#pragma") and not line.startswith("#else") and not line.startswith("#elif") and not line.startswith("#endif"):
            if line == "":
                if import_lines[-1] != "":
                    import_lines.append(line)

                continue

            if not line.startswith("#include"):
                logger.error(f"{file_path}:{i + 1}: Unexpected line found between includes")

                res = False
                continue

            import_lines.append(line)

    import_groups = [[]]

    for line in import_lines:
        if line == "":
            import_groups.append([])

            continue

        import_groups[-1].append(line)

    if len(import_groups) > 3:
        logger.error(f"{file_path}: Amount of include groups is too much")

        res = False

    group_types = []

    for group in import_groups:
        group_type, ok = _get_group_type(group, file_path, lines)
        res &= ok

        group_types.append(group_type)

    for i in range(len(group_types) - 1):
        if group_types[i] > group_types[i + 1]:
            logger.error(f"{file_path}: Invalid include groups order")

            res = False

    return res


def _get_group_type(group, file_path, lines):
    group_type = GroupType.UNSPECIFIED
    res = True

    for line in group:
        include_type = _get_include_type(line, file_path, lines)

        if group_type == GroupType.UNSPECIFIED:
            group_type = include_type
        else:
            if include_type != group_type:
                logger.error(f"{file_path}: Mixing different include types in include group")

                res = False

    return group_type, res


def _get_include_type(line, file_path, lines):
    simplified_file_path = file_path.replace("\\", "/").replace("/test_", "/").replace(".cpp", ".h")
    simplified_include = line.replace('#include "', "").replace('"', "")

    if simplified_include in simplified_file_path:
        return GroupType.MAJOR

    if line.startswith('#include "ui_'):
        return GroupType.MAJOR

    match = major_include_regexp.match(line)

    if match is not None:
        file_name = match.group(1).lower().replace(".h", "")
        search_regexp = re.compile(f".*public {file_name},?$")

        found = False

        for another_line in lines:
            match = search_regexp.match(another_line.lower())

            if match is not None:
                found = True

                break

        if found:
            return GroupType.MAJOR

    if line.startswith("#include <"):
        return GroupType.SYSTEM

    if line.startswith('#include "'):
        return GroupType.PROJECT

    return GroupType.UNSPECIFIED


def is_file_skipped(file_path):
    file_path = file_path.replace("\\", "/")

    skip_files = (
        "/.git/" in file_path or
        "/build/" in file_path or
        "/libs/" in file_path
    )

    return skip_files


def main():
    args = prepare_linter()

    sys.exit(0 if do_validation(args) else 1)


if __name__ == "__main__":
    main()
