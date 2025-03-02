QT += core gui widgets sql websockets

CONFIG += c++20

TARGET = TInvestor
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



CONFIG (debug, debug|release) {
    DEFINES += USE_SANDBOX
}



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror
}
# Warnings as errors - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$${TARGET}/objs
MOC_DIR     = build/gen/$${TARGET}/moc
RCC_DIR     = build/gen/$${TARGET}/rcc
UI_DIR      = build/gen/$${TARGET}/ui
# Target dirs - END



include(../app/TInvestor.pri)
include(../libs/libs.pri)



SOURCES += \
    src/main.cpp

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
