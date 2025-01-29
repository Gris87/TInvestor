QT += core gui widgets sql

CONFIG += c++20

TARGET = tests
TEMPLATE = app



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



include(gtest_dependency.pri)
include(../app/TInvestor.pri)



SOURCES += \
    main.cpp \
    src/config/decisions/buy/test_buydecision1config.cpp \
    src/config/decisions/buy/test_buydecision1configwidget.cpp \
    src/config/decisions/buy/test_buydecision1configwidgetfactory.cpp \
    src/config/decisions/buy/test_buydecision2config.cpp \
    src/config/decisions/buy/test_buydecision2configwidget.cpp \
    src/config/decisions/buy/test_buydecision3config.cpp \
    src/config/decisions/buy/test_buydecision3configwidget.cpp \
    src/config/decisions/sell/test_selldecision1config.cpp \
    src/config/decisions/sell/test_selldecision1configwidget.cpp \
    src/config/decisions/sell/test_selldecision2config.cpp \
    src/config/decisions/sell/test_selldecision2configwidget.cpp \
    src/config/decisions/sell/test_selldecision3config.cpp \
    src/config/decisions/sell/test_selldecision3configwidget.cpp \
    src/config/decisions/test_decisionmakerconfig.cpp \
    src/config/decisions/test_decisionmakerconfigwidget.cpp \
    src/config/test_config.cpp \
    src/config/test_settingsdialog.cpp \
    src/config/test_settingseditor.cpp

HEADERS += \
    src/config/decisions/buy/ibuydecision1config_mock.h \
    src/config/decisions/buy/ibuydecision1configwidget_mock.h \
    src/config/decisions/buy/ibuydecision1configwidgetfactory_mock.h \
    src/config/decisions/buy/ibuydecision2config_mock.h \
    src/config/decisions/buy/ibuydecision3config_mock.h \
    src/config/decisions/idecisionmakerconfig_mock.h \
    src/config/decisions/sell/iselldecision1config_mock.h \
    src/config/decisions/sell/iselldecision2config_mock.h \
    src/config/decisions/sell/iselldecision3config_mock.h \
    src/config/iconfig_mock.h \
    src/config/isettingseditor_mock.h
