QT += core gui widgets

TARGET = TInvestor
TEMPLATE = app



CONFIG += c++20

RC_FILE = Resources.rc
RESOURCES += Resources.qrc

CONFIG (debug, debug|release) {
    DESTDIR = debug/
    OBJECTS_DIR = debug/gen
    MOC_DIR = debug/gen
    RCC_DIR = debug/gen
} else {
    DESTDIR = release/
    OBJECTS_DIR = release/gen
    MOC_DIR = release/gen
    RCC_DIR = release/gen
}



SOURCES +=  src/main.cpp\
            src/main/mainwindow.cpp \
            src/main/trayicon.cpp

HEADERS  += src/main/mainwindow.h \
    src/main/trayicon.h

FORMS    += src/main/mainwindow.ui
