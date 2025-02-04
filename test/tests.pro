QT += core gui widgets sql gui-private

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



# find app/ -name test_\*.cpp | sed "s/app\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    main.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/test_buydecision1configwidget.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/test_buydecision1configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision1config/test_buydecision1config.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/test_buydecision2configwidget.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/test_buydecision2configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision2config/test_buydecision2config.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/test_buydecision3configwidget.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/test_buydecision3configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision3config/test_buydecision3config.cpp \
    src/config/decisions/decisionmakerconfigwidget/test_decisionmakerconfigwidget.cpp \
    src/config/decisions/decisionmakerconfigwidget/test_decisionmakerconfigwidgetfactory.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/test_selldecision1configwidget.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/test_selldecision1configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision1config/test_selldecision1config.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/test_selldecision2configwidget.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/test_selldecision2configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision2config/test_selldecision2config.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/test_selldecision3configwidget.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/test_selldecision3configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision3config/test_selldecision3config.cpp \
    src/config/decisions/test_decisionmakerconfig.cpp \
    src/config/settingsdialog/test_settingsdialog.cpp \
    src/config/settingsdialog/test_settingsdialogfactory.cpp \
    src/config/settingseditor/test_settingseditor.cpp \
    src/config/test_config.cpp \
    src/db/stocks/test_stocksdatabase.cpp \
    src/logger/test_logger.cpp \
    src/main/test_mainwindow.cpp \
    src/storage/stocks/test_stocksstorage.cpp \
    src/threads/cleanup/test_cleanupthread.cpp \
    src/threads/parallelhelper/test_parallelhelperthread.cpp \
    src/threads/refresh/test_refreshthread.cpp \
    src/widgets/trayicon/test_trayicon.cpp \
    src/widgets/trayicon/test_trayiconfactory.cpp

# find app/ -name \*_mock.h | sed "s/app\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget_mock.h \
    src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget_mock.h \
    src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget_mock.h \
    src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h \
    src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h \
    src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget_mock.h \
    src/config/decisions/idecisionmakerconfig_mock.h \
    src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget_mock.h \
    src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget_mock.h \
    src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget_mock.h \
    src/config/iconfig_mock.h \
    src/config/settingsdialog/isettingsdialogfactory_mock.h \
    src/config/settingsdialog/isettingsdialog_mock.h \
    src/config/settingseditor/isettingseditor_mock.h \
    src/db/stocks/istocksdatabase_mock.h \
    src/storage/stocks/istocksstorage_mock.h \
    src/threads/cleanup/icleanupthread_mock.h \
    src/threads/refresh/irefreshthread_mock.h \
    src/widgets/trayicon/itrayiconfactory_mock.h \
    src/widgets/trayicon/itrayicon_mock.h
