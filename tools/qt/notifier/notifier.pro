QT += core gui widgets network sql

CONFIG += c++20

TARGET = Notifier
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror -Wno-comment
}
# Warnings as errors - END

# Extend number of sections - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += -bigobj
}
# Extend number of sections - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



include(../notifier/notifier.pri)
include(../libs/libs.pri)



SOURCES += \
    src/main.cpp
