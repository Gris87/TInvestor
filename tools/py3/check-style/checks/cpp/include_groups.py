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

    start_index = -1
    end_index = -1

    for i, line in enumerate(lines):
        if line.startswith("#include"):
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
                logger.error(f"{file_path}:{i + 1}: unexpected line found between includes")

                res = False
                continue

            import_lines.append(line)

    import_groups = [[]]

    for i, line in enumerate(import_lines):
        if line == "":
            import_groups.append([])

            continue

        import_groups[-1].append(line)

    print(import_groups)

    return res


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
