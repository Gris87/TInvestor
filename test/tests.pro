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
linux-g++ {
    CONFIG (debug, debug|release) {
        QMAKE_CXXFLAGS += "-fprofile-arcs -ftest-coverage"
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



include(../app/TInvestor.pri)
include(../libs/gtest.pri)
include(../libs/libs.pri)
include(../test/tests.pri)



SOURCES += \
    main.cpp
