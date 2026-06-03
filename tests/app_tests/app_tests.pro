QT += core gui widgets charts network sql gui-private

CONFIG += c++20

TARGET = app_tests
TEMPLATE = app



DEFINES += \
    USE_SANDBOX \
    TESTING_MODE



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX /wd4996
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror -Wno-comment
}
# Warnings as errors - END

# Extend number of sections - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += -bigobj
}
# Extend number of sections - END

# Compile with coverage - BEGIN
linux-g++* {
    CONFIG (debug, debug|release) {
        QMAKE_CXXFLAGS += "-fprofile-arcs -ftest-coverage -fno-elide-constructors -fno-default-inline"
        LIBS           += "-lgcov"
    }
}
# Compile with coverage - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



copy_dist_files.commands = echo "Nothing to copy from dist folder"



PROJECT_ROOT_PATH = ../..

include($${PROJECT_ROOT_PATH}/app/app.pri)
include($${PROJECT_ROOT_PATH}/libs/gtest.pri)
include($${PROJECT_ROOT_PATH}/libs/libs.pri)
include($${PROJECT_ROOT_PATH}/tests/app_tests/app_tests.pri)



SOURCES += \
    main.cpp
