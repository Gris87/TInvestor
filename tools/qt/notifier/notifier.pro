QT += core gui widgets network sql

CONFIG += c++20

TARGET = Notifier
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



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

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



copy_dist_files.commands = echo "Nothing to copy from dist folder"



PROJECT_ROOT_PATH = ../../..

include($${PROJECT_ROOT_PATH}/tools/qt/notifier/notifier.pri)
include($${PROJECT_ROOT_PATH}/libs/libs.pri)



SOURCES += \
    src/main.cpp

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
