import argparse
import math
import os
import subprocess
import sys
from colorama import just_fix_windows_console, Fore, Style
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


files_pattern = ["*.h", "*.cpp"]
extra_args = [
    "-checks=signal-with-return-value,heap-allocated-small-trivial-type,ifndef-define-typo,isempty-vs-count,qrequiredresult-candidates,qstring-varargs,qt6-qlatin1stringchar-to-u,qt6-qhash-signature,qt6-header-fixes,qt6-deprecated-api-fixes,qt6-fwd-fixes,tr-non-literal,raw-environment-function,container-inside-loop,qhash-with-char-pointer-key,qvariant-template-instantiation,qproperty-type-mismatch,unneeded-cast,assert-with-side-effects,detaching-member,thread-with-slots,reserve-candidates,jni-signatures,use-arrow-operator-instead-of-data,unexpected-flag-enumerator-value,overloaded-signal,connect-non-signal,qstring-comparison-to-implicit-char,wrong-qevent-cast,lambda-in-connect,lambda-unique-connection,qdatetime-utc,qgetenv,qstring-insensitive-allocation,fully-qualified-moc-types,unused-non-trivial-variable,mutable-container-key,qenums,qmap-with-pointer-key,qstring-ref,strict-iterators,writing-to-temporary,container-anti-pattern,qfileinfo-exists,qstring-arg,empty-qstringliteral,qt-macros,temporary-iterator,wrong-qglobalstatic,lowercase-qml-type-name,no-module-include,use-static-qregularexpression,auto-unexpected-qstringbuilder,connect-3arg-lambda,const-signal-or-slot,detaching-temporary,foreach,install-event-filter,non-pod-global-static,post-event,qdeleteall,qlatin1string-non-ascii,qproperty-without-notify,qstring-left,range-loop-detach,range-loop-reference,returning-data-from-temporary,rule-of-two-soft,child-event-qobject-cast,virtual-signal,overridden-signal,qhash-namespace,skipped-base-method,ctor-missing-parent-argument",
    "-p",
    Path(Path(os.getcwd()) / "build/Desktop-Debug/.qtc_clangd").absolute(),
]


def run_clazy(args):
    just_fix_windows_console()

    paths = []
    for pattern in files_pattern:
        paths.extend(Path("app").rglob(pattern))
        paths.extend(Path("test").rglob(pattern))

    matched_files = sorted([str(path.absolute()) for path in paths if path.is_file()])

    number_of_parts = min(max(args.number_of_parts, 1), len(matched_files))
    part = min(max(args.part, 1), number_of_parts)

    chunk_size = math.ceil(len(matched_files) / number_of_parts);
    file_chunks = [matched_files[i : i + chunk_size] for i in range(0, len(matched_files), chunk_size)]

    commands = []

    for file_path in file_chunks[part - 1]:
        command = ["clazy-standalone"]
        command.extend(extra_args)
        command.append(file_path)

        commands.append(command)

    return _execute_commands(commands)


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for file_path, result, lines in executor.map(_execute_command, commands):
            print(f"Analyzed {file_path}")

            if result:
                for line in lines:
                    print(line)
            else:
                res = False

                for line in lines:
                    print(Fore.RED + line + Style.RESET_ALL)

    return res


def _execute_command(command):
    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
    )

    lines = []

    encoding = os.device_encoding(1)
    if encoding is None:
        encoding = "utf-8"

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return command[-1], process.returncode == 0, lines


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--part",
        dest="part",
        type=int,
        default=1,
        help="Part index"
    )
    parser.add_argument(
        "--number-of-parts",
        dest="number_of_parts",
        type=int,
        default=1,
        help="Amount of parts"
    )
    args = parser.parse_args()

    if run_clazy(args):
        print("SUCCESS")

        sys.exit(0)

    print("FAILURE")

    sys.exit(1)
