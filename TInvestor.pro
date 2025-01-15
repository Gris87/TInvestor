QT += core gui widgets

CONFIG += c++20

TARGET = TInvestor
TEMPLATE = app



RC_FILE = Resources.rc
RESOURCES += Resources.qrc



# Warnings as errors - BEGIN
win32-msvc* {
    QMAKE_CXXFLAGS += /WX
} else {
    QMAKE_CXXFLAGS += -Wall -Wextra -Werror
}
# Warnings as errors - END

# Release optimization - BEGIN
QMAKE_CFLAGS_RELEASE -= -O1
QMAKE_CFLAGS_RELEASE -= -O2
QMAKE_CFLAGS_RELEASE += -O3

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -O3
# Release optimization - END

# Target dirs - BEGIN
DESTDIR     = build/
OBJECTS_DIR = build/gen/$$TARGET/objs
MOC_DIR     = build/gen/$$TARGET/moc
RCC_DIR     = build/gen/$$TARGET/rcc
UI_DIR      = build/gen/$$TARGET/ui
# Target dirs - END



SOURCES += \
    src/config/config.cpp \
    src/config/settingsdialog.cpp \
    src/logger/logger.cpp \
    src/main.cpp\
    src/main/mainwindow.cpp \
    src/main/trayicon.cpp

HEADERS += \
    src/config/config.h \
    src/config/settingsdialog.h \
    src/logger/logger.h \
    src/main/mainwindow.h \
    src/main/trayicon.h

FORMS += \
    src/config/settingsdialog.ui \
    src/main/mainwindow.ui

TRANSLATIONS += \
    assets/translations/language_en.ts \
    assets/translations/language_ru.ts
