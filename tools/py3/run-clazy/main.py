import argparse
import math
import os
import subprocess
import sys
from colorama import just_fix_windows_console, Fore, Style
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


all_checks = "qt-keywords,qt-keyword-emit,signal-with-return-value,sanitize-inline-keyword,heap-allocated-small-trivial-type,ifndef-define-typo,isempty-vs-count,qrequiredresult-candidates,qstring-varargs,qt6-qlatin1stringchar-to-u,qt6-qhash-signature,qt6-header-fixes,qt6-deprecated-api-fixes,qt6-fwd-fixes,tr-non-literal,raw-environment-function,container-inside-loop,qhash-with-char-pointer-key,qvariant-template-instantiation,qproperty-type-mismatch,unneeded-cast,assert-with-side-effects,detaching-member,thread-with-slots,reserve-candidates,jni-signatures,use-chrono-in-qtimer,use-arrow-operator-instead-of-data,unexpected-flag-enumerator-value,unused-result-check,overloaded-signal,connect-by-name,connect-non-signal,qstring-comparison-to-implicit-char,wrong-qevent-cast,lambda-in-connect,lambda-unique-connection,qdatetime-utc,qgetenv,qstring-insensitive-allocation,fully-qualified-moc-types,unused-non-trivial-variable,connect-not-normalized,mutable-container-key,qenums,qmap-with-pointer-key,qstring-ref,strict-iterators,writing-to-temporary,container-anti-pattern,qcolor-from-literal,qfileinfo-exists,qstring-arg,empty-qstringliteral,qt-macros,temporary-iterator,wrong-qglobalstatic,lowercase-qml-type-name,no-module-include,use-static-qregularexpression,auto-unexpected-qstringbuilder,connect-3arg-lambda,const-signal-or-slot,detaching-temporary,foreach,incorrect-emit,install-event-filter,non-pod-global-static,post-event,qdeleteall,qlatin1string-non-ascii,qproperty-without-notify,qstring-left,range-loop-detach,range-loop-reference,returning-data-from-temporary,rule-of-two-soft,child-event-qobject-cast,virtual-signal,overridden-signal,qhash-namespace,skipped-base-method,ctor-missing-parent-argument,base-class-event,copyable-polymorphic,function-args-by-ref,function-args-by-value,global-const-char-pointer,implicit-casts,missing-qobject-macro,missing-typeinfo,old-style-connect,qstring-allocations,returning-void-expression,rule-of-three,virtual-call-ctor,static-pmf".replace("\n", "").replace(" ", "").split(",")

disabled_checks = """
-*,
*,
-connect-by-name,
-connect-not-normalized,
-function-args-by-value,
-old-style-connect,
-qcolor-from-literal,
-qstring-allocations,
-qt-keyword-emit,
-qt-keywords,
-qt6-qhash-signature,
-reserve-candidates,
-returning-void-expression,
-unused-result-check
""".replace("\n", "").replace(" ", "").split(",")


files_pattern = ["*.h", "*.cpp"]
extra_args = [
    "-p",
    Path(Path(os.getcwd()) / "build/Desktop-Debug/.qtc_clangd").absolute(),
    '--header-filter=".*src/.*"',
]


def run_clazy(args):
    just_fix_windows_console()

    paths = []
    for pattern in files_pattern:
        paths.extend(Path("app").rglob(pattern))

    matched_files = sorted([str(path.absolute()) for path in paths if path.is_file() and not path.name.startswith("test_")])
    checks = _get_checks()

    number_of_parts = min(max(args.number_of_parts, 1), len(checks))
    part = min(max(args.part, 1), number_of_parts)

    chunk_size = math.ceil(len(checks) / number_of_parts);
    check_chunks = [checks[i : i + chunk_size] for i in range(0, len(checks), chunk_size)]
    chunk = check_chunks[part - 1]

    print(f"Checks in chunk: {len(chunk)}")
    print(f"Files to check: {len(matched_files)}")

    commands = []

    for check in chunk:
        for file_path in matched_files:
            command = ["clazy-standalone"]
            command.append(f"-checks={check}")
            command.extend(extra_args)
            command.append(file_path)

            commands.append(command)

    return _execute_commands(commands)


def _execute_commands(commands):
    res = True

    with ThreadPoolExecutor(os.cpu_count()) as executor:
        for check, file_path, result, lines in executor.map(_execute_command, commands):
            if result:
                print(f"Analyzed {file_path} with {check}")

                for line in lines:
                    print(line)
            else:
                res = False
                print(f"Analyzed {file_path} with {check} [FAILED]")

                for line in lines:
                    print(Fore.RED + line + Style.RESET_ALL)

    return res


def _execute_command(command):
    env = os.environ.copy()
    env["CLAZY_CHECKS_AS_ERRORS"] = command[1].split("=")[1]

    process = subprocess.Popen(
        command,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        env=env,
    )

    lines = []

    encoding = os.device_encoding(1)
    if encoding is None:
        encoding = "utf-8"

    for line in iter(process.stdout.readline, b''):
        lines.append(line.rstrip().decode(encoding))

    process.wait()

    return command[1].split("=")[1], command[-1], process.returncode == 0, lines


def _get_checks():
    res = [x for x in all_checks if f"-{x}" not in disabled_checks]
    # print(",".join(res)) # For debug only

    return res


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
