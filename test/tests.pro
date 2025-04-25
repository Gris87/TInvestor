QT += core gui widgets network sql gui-private

CONFIG += c++20

TARGET = tests
TEMPLATE = app



DEFINES += \
    USE_SANDBOX



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror
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



linux* {
    copy_dist_files.commands = echo "Nothing to copy from dist folder"
}



include(../app/TInvestor.pri)
include(../libs/gtest.pri)
include(../libs/libs.pri)
include(../test/tests.pri)



SOURCES += \
    main.cpp
