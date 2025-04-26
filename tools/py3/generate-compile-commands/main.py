import argparse
import json
import os
import sys

from pathlib import Path


cwd = os.getcwd()


def generate(args):
    commands = _generate_compile_commands(args)
    qtc_commands = _load_qtc_compile_commands(args)

    _compare_compile_commands(commands, qtc_commands)

    return _store_compile_commands(args, commands)


def _generate_compile_commands(args):
    res = []

    directory = str(Path(args.directory).absolute()).replace("\\", "/")

    paths = []
    paths.extend(Path(".").rglob("*.h"))
    paths.extend(Path(".").rglob("*.cpp"))
    paths.extend(Path(".").rglob("*.cc"))

    for path in paths:
        file_path = str(path.absolute()).replace("\\", "/")

        need_to_skip = "/build/gen/" in file_path

        if not need_to_skip and args.ci:
            need_to_skip = "/libs/" in file_path or "/test_" in file_path

        if need_to_skip:
            continue

        if "_mock.h" not in file_path and "/test_" not in file_path and "/test/main.cpp" not in file_path:
            res.append({
                "arguments": _get_arguments_for_file(args, file_path, False),
                "directory": directory,
                "file": file_path,
            })

        if ("/app/src/" in file_path and "/app/src/main.cpp" not in file_path) or "/test/main.cpp" in file_path:
            res.append({
                "arguments": _get_arguments_for_file(args, file_path, True),
                "directory": directory,
                "file": file_path,
            })

    for path in Path(".").rglob("*.ui"):
        header_file = path.name.replace(".ui", ".h")
        app_file_path = f"{cwd}/build/Desktop-Debug/app/build/gen/TInvestor/ui/ui_{header_file}".replace("\\", "/")
        test_file_path = f"{cwd}/build/Desktop-Debug/test/build/gen/tests/ui/ui_{header_file}".replace("\\", "/")

        res.append({
            "arguments": _get_arguments_for_file(args, app_file_path, False),
            "directory": directory,
            "file": app_file_path,
        })
        res.append({
            "arguments": _get_arguments_for_file(args, test_file_path, True),
            "directory": directory,
            "file": test_file_path,
        })

    gmock_all = f"{args.google_test_path}/googlemock/src/gmock-all.cc".replace("\\", "/")
    gtest_all = f"{args.google_test_path}/googletest/src/gtest-all.cc".replace("\\", "/")

    res.append({
        "arguments": _get_arguments_for_file(args, gmock_all, True),
        "directory": directory,
        "file": gmock_all,
    })
    res.append({
        "arguments": _get_arguments_for_file(args, gtest_all, True),
        "directory": directory,
        "file": gtest_all,
    })

    res.sort(key=lambda x: (x["file"], x["arguments"]))

    return res


def _get_arguments_for_file(args, file_path, duplicate_for_tests):
    return _get_arguments_for_file_windows(args, file_path, duplicate_for_tests) if args.operation_system == "Windows" else _get_arguments_for_file_linux(args, file_path, duplicate_for_tests)


def _get_arguments_for_file_windows(args, file_path, duplicate_for_tests):
    res = []

    app_or_test = "/app/src/" in file_path or "/test/main.cpp" in file_path or "/app/build/gen/" in file_path or "/test/build/gen/" in file_path or "/googletest/src/" in file_path or "/googlemock/src/" in file_path

    res.append("clang")
    res.append("-Wno-documentation-unknown-command")
    res.append("-Wno-unknown-warning-option")
    res.append("-Wno-unknown-pragmas")
    res.append("-nostdinc")

    if not args.ci:
        res.append("-nostdinc++")

    res.append("--driver-mode=cl")
    res.append("-nologo")
    res.append("-Zc:wchar_t")
    res.append("-FS")
    res.append("-Zc:rvalueCast")
    res.append("-Zc:inline")
    res.append("-Zc:strictStrings")

    if not args.ci:
        res.append("-Zc:throwingNew")

    res.append("-permissive-")
    res.append("-Zc:__cplusplus")

    if not args.ci:
        res.append("-Zc:externConstexpr")

    if app_or_test and "/libs/" not in file_path:
        if args.target == "build":
            res.append("-WX")

    res.append("-bigobj")
    res.append("-Zi")
    res.append("-MDd")
    res.append("-clang:-std=c++20")
    res.append("-utf-8")

    if args.target == "build":
        if app_or_test and "/libs/" not in file_path:
            res.append("-W3")
            res.append("-w34100")
            res.append("-w34189")
            res.append("-w44456")
            res.append("-w44457")
            res.append("-w44458")
            res.append("-wd4577")
            res.append("-wd4467")
        else:
            res.append("-w")

    res.append("-EHsc")
    res.append("/Zs")
    res.append("-m64")
    res.append("--target=x86_64-pc-windows-msvc")

    if not args.ci:
        res.append("-fcxx-exceptions")
        res.append("-fexceptions")

    res.append("-fms-compatibility-version=19.42")
    res.append("-DUNICODE")
    res.append("-D_UNICODE")
    res.append("-DWIN32")
    res.append("-D_ENABLE_EXTENDED_ALIGNED_STORAGE")
    res.append("-DWIN64")

    if app_or_test and "/libs/" not in file_path:
        res.append("-DUSE_SANDBOX")

    res.append("-DQT_QML_DEBUG")

    if app_or_test and "/libs/" not in file_path:
        res.append("-DQT_WIDGETS_LIB")
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_NETWORK_LIB")
        res.append("-DQT_SQL_LIB")
        res.append("-DQT_CORE_LIB")

    if "/libs/simplecrypt/" in file_path:
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_CORE_LIB")

    if "/libs/verticallabel/" in file_path:
        res.append("-DQT_WIDGETS_LIB")
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_CORE_LIB")

    if "/libs/waitingspinner/" in file_path:
        res.append("-DQT_WIDGETS_LIB")
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_CORE_LIB")

    if args.target == "build":
        res.append("-DQ_CREATOR_RUN")

    res.append("-D__FUNCSIG__=\"void __cdecl someLegalAndLongishFunctionNameThatWorksAroundQTCREATORBUG-24580(void)\"")
    res.append("-D__FUNCTION__=\"someLegalAndLongishFunctionNameThatWorksAroundQTCREATORBUG-24580\"")
    res.append("-D__FUNCDNAME__=\"?someLegalAndLongishFunctionNameThatWorksAroundQTCREATORBUG-24580@@YAXXZ\"")

    if app_or_test and "/libs/" not in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders\\QtCore")

        if duplicate_for_tests:
            res.append(f"-I{cwd}\\test")

        res.append(f"-I{cwd}\\app")

        if duplicate_for_tests:
            res.append(f"-I{args.google_test_path}\\googletest")
            res.append(f"-I{args.google_test_path}\\googletest\\include")
            res.append(f"-I{args.google_test_path}\\googlemock")
            res.append(f"-I{args.google_test_path}\\googlemock\\include")

        res.append(f"-I{cwd}\\libs\\investapi")
        res.append(f"-I{cwd}\\libs\\investapi\\messages\\generated")
        res.append(f"-I{args.vcpkg_path}\\installed\\x64-windows\\include")
        res.append(f"-I{args.quazip_path}")
        res.append(f"-I{cwd}\\libs\\simplecrypt")
        res.append(f"-I{cwd}\\libs\\verticallabel")
        res.append(f"-I{cwd}\\libs\\waitingspinner")
        res.append(f"-I{args.qt_path}\\include")
        res.append(f"-I{args.qt_path}\\include\\QtWidgets")

        if duplicate_for_tests:
            res.append(f"-I{args.qt_path}\\include\\QtGui\\6.9.0")
            res.append(f"-I{args.qt_path}\\include\\QtGui\\6.9.0\\QtGui")

        res.append(f"-I{args.qt_path}\\include\\QtGui")
        res.append(f"-I{args.qt_path}\\include\\QtNetwork")
        res.append(f"-I{args.qt_path}\\include\\QtSql")

        if duplicate_for_tests:
            res.append(f"-I{args.qt_path}\\include\\QtCore\\6.9.0")
            res.append(f"-I{args.qt_path}\\include\\QtCore\\6.9.0\\QtCore")

        res.append(f"-I{args.qt_path}\\include\\QtCore")

        if duplicate_for_tests:
            res.append(f"-I{cwd}\\build\\Desktop-Debug\\test\\build\\gen\\tests\\moc")
            res.append(f"-I{cwd}\\build\\Desktop-Debug\\test\\build\\gen\\tests\\ui")
            res.append(f"-I{cwd}\\build\\Desktop-Debug\\test")
        else:
            res.append(f"-I{cwd}\\build\\Desktop-Debug\\app\\build\\gen\\TInvestor\\moc")
            res.append(f"-I{cwd}\\build\\Desktop-Debug\\app\\build\\gen\\TInvestor\\ui")
            res.append(f"-I{cwd}\\build\\Desktop-Debug\\app")

    if "/libs/investapi/" in file_path:
        res.append(f"-I{cwd}\\libs\\investapi")
        res.append(f"-I{cwd}\\libs\\investapi\\messages\\generated")
        res.append(f"-I{args.vcpkg_path}\\installed\\x64-windows\\include")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\investapi")

    if "/libs/simplecrypt/" in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders\\QtCore")

        res.append(f"-I{cwd}\\libs\\simplecrypt")
        res.append(f"-I{args.qt_path}\\include")
        res.append(f"-I{args.qt_path}\\include\\QtGui")
        res.append(f"-I{args.qt_path}\\include\\QtCore")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\simplecrypt\\build\\gen\\simplecrypt\\moc")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\simplecrypt")

    if "/libs/verticallabel/" in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders\\QtCore")

        res.append(f"-I{cwd}\\libs\\verticallabel")
        res.append(f"-I{args.qt_path}\\include")
        res.append(f"-I{args.qt_path}\\include\\QtWidgets")
        res.append(f"-I{args.qt_path}\\include\\QtGui")
        res.append(f"-I{args.qt_path}\\include\\QtCore")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\verticallabel\\build\\gen\\verticallabel\\moc")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\verticallabel")

    if "/libs/waitingspinner/" in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}\\share\\qtcreator\\cplusplus\\wrappedQtHeaders\\QtCore")

        res.append(f"-I{cwd}\\libs\\waitingspinner")
        res.append(f"-I{args.qt_path}\\include")
        res.append(f"-I{args.qt_path}\\include\\QtWidgets")
        res.append(f"-I{args.qt_path}\\include\\QtGui")
        res.append(f"-I{args.qt_path}\\include\\QtCore")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\waitingspinner\\build\\gen\\waitingspinner\\moc")
        res.append(f"-I{cwd}\\build\\Desktop-Debug\\libs\\waitingspinner")

    res.append(f"-I{args.qt_path}\\mkspecs\\win32-msvc")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.qt_creator_path}\\bin\\clang\\lib\\clang\\19\\include")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.msvc_tools_path}\\include")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.msvc_auxiliary_path}\\VS\\include")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.windows_kits_path}\\ucrt")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.windows_kits_path}\\um")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.windows_kits_path}\\shared")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.windows_kits_path}\\winrt")
    res.append("/clang:-isystem")
    res.append(f"/clang:{args.windows_kits_path}\\cppwinrt")

    if args.target == "build":
        res.append("/clang:-fmessage-length=0")
        res.append("/clang:-fdiagnostics-show-note-include-stack")
        res.append("/clang:-fretain-comments-from-system-headers")
        res.append("-fmacro-backtrace-limit=0")
        res.append("-ferror-limit=1000")

    res.append("/TP")

    if file_path.endswith(".h"):
        if args.target != "build":
            res.append("-Wno-pragma-once-outside-header")

    res.append(file_path.replace("/", "\\"))

    return res


def _get_arguments_for_file_linux(args, file_path, duplicate_for_tests):
    res = []

    app_or_test = "/app/src/" in file_path or "/test/main.cpp" in file_path or "/app/build/gen/" in file_path or "/test/build/gen/" in file_path or "/googletest/src/" in file_path or "/googlemock/src/" in file_path

    res.append("clang")
    res.append("-Wno-documentation-unknown-command")
    res.append("-Wno-unknown-warning-option")
    res.append("-Wno-unknown-pragmas")
    res.append("-nostdinc")
    res.append("-nostdinc++")
    res.append("-pipe")

    if app_or_test and "/libs/" not in file_path:
        if args.target == "build":
            res.append("-Wall")
            res.append("-Wextra")
            res.append("-Werror")

    if duplicate_for_tests:
        res.append("-fprofile-arcs")
        res.append("-ftest-coverage")
        res.append("-fno-elide-constructors")
        res.append("-fno-default-inline")

    res.append("-g")

    if not(app_or_test and "/libs/" not in file_path):
        res.append("-fPIC")

    res.append("-std=gnu++2a")

    if args.target == "build":
        res.append("-Wall")
        res.append("-Wextra")

    if app_or_test and "/libs/" not in file_path:
        res.append("-fPIC")
        res.append("-D_REENTRANT")
    else:
        if args.target == "build":
            res.append("-w")

    if "/libs/simplecrypt/" in file_path:
        res.append("-D_REENTRANT")

    if "/libs/verticallabel/" in file_path:
        res.append("-D_REENTRANT")

    if "/libs/waitingspinner/" in file_path:
        res.append("-D_REENTRANT")

    res.append("-fsyntax-only")
    res.append("-m64")
    res.append("--target=x86_64-linux-gnu")

    if app_or_test and "/libs/" not in file_path:
        res.append("-DUSE_SANDBOX")

    res.append("-DQT_QML_DEBUG")

    if app_or_test and "/libs/" not in file_path:
        res.append("-DQT_WIDGETS_LIB")
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_NETWORK_LIB")
        res.append("-DQT_SQL_LIB")
        res.append("-DQT_CORE_LIB")

    if "/libs/simplecrypt/" in file_path:
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_CORE_LIB")

    if "/libs/verticallabel/" in file_path:
        res.append("-DQT_WIDGETS_LIB")
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_CORE_LIB")

    if "/libs/waitingspinner/" in file_path:
        res.append("-DQT_WIDGETS_LIB")
        res.append("-DQT_GUI_LIB")
        res.append("-DQT_CORE_LIB")

    if args.target == "build":
        res.append("-DQ_CREATOR_RUN")

    if app_or_test and "/libs/" not in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders/QtCore")

        if duplicate_for_tests:
            res.append(f"-I{cwd}/test")

        res.append(f"-I{cwd}/app")

        if duplicate_for_tests:
            res.append(f"-I{args.google_test_path}/googletest")
            res.append(f"-I{args.google_test_path}/googletest/include")
            res.append(f"-I{args.google_test_path}/googlemock")
            res.append(f"-I{args.google_test_path}/googlemock/include")

        res.append(f"-I{cwd}/libs/investapi")
        res.append(f"-I{cwd}/libs/investapi/messages/generated")
        res.append(f"-I{args.vcpkg_path}/installed/x64-linux/include")
        res.append(f"-I{args.quazip_path}")
        res.append(f"-I{cwd}/libs/simplecrypt")
        res.append(f"-I{cwd}/libs/verticallabel")
        res.append(f"-I{cwd}/libs/waitingspinner")
        res.append(f"-I{args.qt_path}/include")
        res.append(f"-I{args.qt_path}/include/QtWidgets")

        if duplicate_for_tests:
            res.append(f"-I{args.qt_path}/include/QtGui/6.9.0")
            res.append(f"-I{args.qt_path}/include/QtGui/6.9.0/QtGui")

        res.append(f"-I{args.qt_path}/include/QtGui")
        res.append(f"-I{args.qt_path}/include/QtNetwork")
        res.append(f"-I{args.qt_path}/include/QtSql")

        if duplicate_for_tests:
            res.append(f"-I{args.qt_path}/include/QtCore/6.9.0")
            res.append(f"-I{args.qt_path}/include/QtCore/6.9.0/QtCore")

        res.append(f"-I{args.qt_path}/include/QtCore")

        if duplicate_for_tests:
            res.append(f"-I{cwd}/build/Desktop-Debug/test/build/gen/tests/moc")
            res.append(f"-I{cwd}/build/Desktop-Debug/test/build/gen/tests/ui")
            res.append(f"-I{cwd}/build/Desktop-Debug/test")
        else:
            res.append(f"-I{cwd}/build/Desktop-Debug/app/build/gen/TInvestor/moc")
            res.append(f"-I{cwd}/build/Desktop-Debug/app/build/gen/TInvestor/ui")
            res.append(f"-I{cwd}/build/Desktop-Debug/app")

    if "/libs/investapi/" in file_path:
        res.append(f"-I{cwd}/libs/investapi")
        res.append(f"-I{cwd}/libs/investapi/messages/generated")
        res.append(f"-I{args.vcpkg_path}/installed/x64-linux/include")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/investapi")

    if "/libs/simplecrypt/" in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders/QtCore")

        res.append(f"-I{cwd}/libs/simplecrypt")
        res.append(f"-I{args.qt_path}/include")
        res.append(f"-I{args.qt_path}/include/QtGui")
        res.append(f"-I{args.qt_path}/include/QtCore")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/simplecrypt/build/gen/simplecrypt/moc")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/simplecrypt")

    if "/libs/verticallabel/" in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders/QtCore")

        res.append(f"-I{cwd}/libs/verticallabel")
        res.append(f"-I{args.qt_path}/include")
        res.append(f"-I{args.qt_path}/include/QtWidgets")
        res.append(f"-I{args.qt_path}/include/QtGui")
        res.append(f"-I{args.qt_path}/include/QtCore")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/verticallabel/build/gen/verticallabel/moc")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/verticallabel")

    if "/libs/waitingspinner/" in file_path:
        res.append("-DQT_ANNOTATE_FUNCTION(x)=__attribute__((annotate(#x)))")

        if args.target == "build":
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders")
            res.append(f"-I{args.qt_creator_path}/share/qtcreator/cplusplus/wrappedQtHeaders/QtCore")

        res.append(f"-I{cwd}/libs/waitingspinner")
        res.append(f"-I{args.qt_path}/include")
        res.append(f"-I{args.qt_path}/include/QtWidgets")
        res.append(f"-I{args.qt_path}/include/QtGui")
        res.append(f"-I{args.qt_path}/include/QtCore")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/waitingspinner/build/gen/waitingspinner/moc")
        res.append(f"-I{cwd}/build/Desktop-Debug/libs/waitingspinner")

    res.append(f"-I{args.qt_path}/mkspecs/linux-g++")

    if not duplicate_for_tests:
        res.append("-isystem")
        res.append("/usr/include/c++/13")
        res.append("-isystem")
        res.append("/usr/include/x86_64-linux-gnu/c++/13")
        res.append("-isystem")
        res.append("/usr/include/c++/13/backward")
        res.append("-isystem")
        res.append("/usr/local/include")

    res.append("-isystem")
    res.append(f"{args.qt_creator_path}/libexec/qtcreator/clang/lib/clang/19/include")

    if not duplicate_for_tests:
        res.append("-isystem")
        res.append("/usr/include/x86_64-linux-gnu")
        res.append("-isystem")
        res.append("/usr/include")

    if args.target == "build":
        res.append("-fmessage-length=0")
        res.append("-fdiagnostics-show-note-include-stack")
        res.append("-fretain-comments-from-system-headers")
        res.append("-fmacro-backtrace-limit=0")
        res.append("-ferror-limit=1000")

    res.append("-x")

    if file_path.endswith(".h"):
        res.append("c++-header")

        if args.target != "build":
            res.append("-Wno-pragma-once-outside-header")
    else:
        res.append("c++")

    res.append(file_path)

    return res


def _load_qtc_compile_commands(args):
    res = None

    try:
        with open(args.qtc_commands, "r") as f:
            res = json.load(f)

        res.sort(key=lambda x: (x["file"], x["arguments"]))

        with open(args.qtc_commands, "w") as f:
            f.write(json.dumps(res, indent=4))
    except Exception as e:
        return None

    return res


def _compare_compile_commands(commands, qtc_commands):
    if qtc_commands is not None:
        commands_text = json.dumps(commands, indent=4)
        qtc_commands_text = json.dumps(qtc_commands, indent=4)

        if commands_text != qtc_commands_text:
            print("Generated:")
            print(commands_text)
            print("")
            print("But expecting for:")
            print(qtc_commands_text)


def _store_compile_commands(args, commands):
    Path(args.output).parent.mkdir(parents=True, exist_ok=True)

    with open(args.output, "w") as f:
        f.write(json.dumps(commands, indent=4))

    return True


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "--os",
        dest="operation_system",
        type=str,
        choices=["Windows", "Linux"],
        default="Windows",
        help="Operation system",
    )
    parser.add_argument(
        "--target",
        dest="target",
        type=str,
        choices=["build", "clang-tidy", "clazy"],
        default="build",
        help="Target",
    )
    parser.add_argument(
        "--ci",
        default=False,
        action="store_true",
        help="Flag for CI",
    )
    parser.add_argument(
        "--qt-path",
        dest="qt_path",
        type=str,
        default="E:\\Qt\\6.9.0\\msvc2022_64",
        help="Path to Qt",
    )
    parser.add_argument(
        "--qt-creator-path",
        dest="qt_creator_path",
        type=str,
        default="E:\\Qt\\qtcreator-windows-x64-msvc-15.0.0",
        help="Path to Qt Creator",
    )
    parser.add_argument(
        "--quazip-path",
        dest="quazip_path",
        type=str,
        default="E:\\Qt\\quazip-1.5\\install\\include\\QuaZip-Qt6-1.5",
        help="Path to quazip",
    )
    parser.add_argument(
        "--vcpkg-path",
        dest="vcpkg_path",
        type=str,
        default="E:\\Qt\\vcpkg",
        help="Path to vcpkg",
    )
    parser.add_argument(
        "--google-test-path",
        dest="google_test_path",
        type=str,
        default="E:\\Qt\\googletest-1.16.0",
        help="Path to Google Test",
    )
    parser.add_argument(
        "--msvc-tools-path",
        dest="msvc_tools_path",
        type=str,
        default="E:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\MSVC\\14.42.34433",
        help="Path to MSVC Tools",
    )
    parser.add_argument(
        "--msvc-auxiliary-path",
        dest="msvc_auxiliary_path",
        type=str,
        default="E:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Auxiliary",
        help="Path to MSVC Auxiliary",
    )
    parser.add_argument(
        "--windows-kits-path",
        dest="windows_kits_path",
        type=str,
        default="C:\\Program Files (x86)\\Windows Kits\\10\\include\\10.0.22621.0",
        help="Path to Windows Kits",
    )
    parser.add_argument(
        "--directory",
        dest="directory",
        type=str,
        default="build/Desktop-Debug/.qtc_clangd",
        help="Directory to insert into compile_commands.json",
    )
    parser.add_argument(
        "--qtc-commands",
        dest="qtc_commands",
        type=str,
        default="build/Desktop-Debug/.qtc_clangd/compile_commands.json",
        help="compile_commands.json generated by Qt Creator",
    )
    parser.add_argument(
        "--output",
        dest="output",
        type=str,
        default="build/Desktop-Debug/compile_commands.json",
        help="Output file"
    )
    args = parser.parse_args()

    sys.exit(0 if generate(args) else 1)
