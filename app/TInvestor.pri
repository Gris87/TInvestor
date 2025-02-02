VPATH = $$PWD
INCLUDEPATH += $$PWD



SOURCES += \
    src/config/config.cpp \
    src/config/decisions/buy/buydecision1config.cpp \
    src/config/decisions/buy/buydecision1configwidget.cpp \
    src/config/decisions/buy/buydecision1configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision2config.cpp \
    src/config/decisions/buy/buydecision2configwidget.cpp \
    src/config/decisions/buy/buydecision2configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision3config.cpp \
    src/config/decisions/buy/buydecision3configwidget.cpp \
    src/config/decisions/buy/buydecision3configwidgetfactory.cpp \
    src/config/decisions/decisionmakerconfig.cpp \
    src/config/decisions/decisionmakerconfigwidget.cpp \
    src/config/decisions/decisionmakerconfigwidgetfactory.cpp \
    src/config/decisions/sell/selldecision1config.cpp \
    src/config/decisions/sell/selldecision1configwidget.cpp \
    src/config/decisions/sell/selldecision1configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision2config.cpp \
    src/config/decisions/sell/selldecision2configwidget.cpp \
    src/config/decisions/sell/selldecision2configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision3config.cpp \
    src/config/decisions/sell/selldecision3configwidget.cpp \
    src/config/decisions/sell/selldecision3configwidgetfactory.cpp \
    src/config/settingsdialog.cpp \
    src/config/settingsdialogfactory.cpp \
    src/config/settingseditor.cpp \
    src/db/stocks/stocksdatabase.cpp \
    src/logger/logger.cpp \
    src/main/mainwindow.cpp \
    src/main/trayicon.cpp \
    src/main/trayiconfactory.cpp \
    src/storage/stockstorage.cpp \
    src/threads/cleanupthread.cpp \
    src/threads/refreshthread.cpp

HEADERS += \
    src/config/config.h \
    src/config/decisions/buy/buydecision1config.h \
    src/config/decisions/buy/buydecision1configwidget.h \
    src/config/decisions/buy/buydecision1configwidgetfactory.h \
    src/config/decisions/buy/buydecision2config.h \
    src/config/decisions/buy/buydecision2configwidget.h \
    src/config/decisions/buy/buydecision2configwidgetfactory.h \
    src/config/decisions/buy/buydecision3config.h \
    src/config/decisions/buy/buydecision3configwidget.h \
    src/config/decisions/buy/buydecision3configwidgetfactory.h \
    src/config/decisions/buy/ibuydecision1config.h \
    src/config/decisions/buy/ibuydecision1configwidget.h \
    src/config/decisions/buy/ibuydecision1configwidgetfactory.h \
    src/config/decisions/buy/ibuydecision2config.h \
    src/config/decisions/buy/ibuydecision2configwidget.h \
    src/config/decisions/buy/ibuydecision2configwidgetfactory.h \
    src/config/decisions/buy/ibuydecision3config.h \
    src/config/decisions/buy/ibuydecision3configwidget.h \
    src/config/decisions/buy/ibuydecision3configwidgetfactory.h \
    src/config/decisions/decisionmakerconfig.h \
    src/config/decisions/decisionmakerconfigwidget.h \
    src/config/decisions/decisionmakerconfigwidgetfactory.h \
    src/config/decisions/idecisionmakerconfig.h \
    src/config/decisions/idecisionmakerconfigwidget.h \
    src/config/decisions/idecisionmakerconfigwidgetfactory.h \
    src/config/decisions/sell/iselldecision1config.h \
    src/config/decisions/sell/iselldecision1configwidget.h \
    src/config/decisions/sell/iselldecision1configwidgetfactory.h \
    src/config/decisions/sell/iselldecision2config.h \
    src/config/decisions/sell/iselldecision2configwidget.h \
    src/config/decisions/sell/iselldecision2configwidgetfactory.h \
    src/config/decisions/sell/iselldecision3config.h \
    src/config/decisions/sell/iselldecision3configwidget.h \
    src/config/decisions/sell/iselldecision3configwidgetfactory.h \
    src/config/decisions/sell/selldecision1config.h \
    src/config/decisions/sell/selldecision1configwidget.h \
    src/config/decisions/sell/selldecision1configwidgetfactory.h \
    src/config/decisions/sell/selldecision2config.h \
    src/config/decisions/sell/selldecision2configwidget.h \
    src/config/decisions/sell/selldecision2configwidgetfactory.h \
    src/config/decisions/sell/selldecision3config.h \
    src/config/decisions/sell/selldecision3configwidget.h \
    src/config/decisions/sell/selldecision3configwidgetfactory.h \
    src/config/iconfig.h \
    src/config/isettingsdialog.h \
    src/config/isettingsdialogfactory.h \
    src/config/isettingseditor.h \
    src/config/settingsdialog.h \
    src/config/settingsdialogfactory.h \
    src/config/settingseditor.h \
    src/db/stocks/istocksdatabase.h \
    src/db/stocks/stocksdatabase.h \
    src/domain/stocks/stock.h \
    src/logger/logger.h \
    src/main/itrayicon.h \
    src/main/itrayiconfactory.h \
    src/main/mainwindow.h \
    src/main/trayicon.h \
    src/main/trayiconfactory.h \
    src/storage/istockstorage.h \
    src/storage/stockstorage.h \
    src/threads/cleanupthread.h \
    src/threads/icleanupthread.h \
    src/threads/irefreshthread.h \
    src/threads/refreshthread.h

FORMS += \
    src/config/decisions/buy/buydecision1configwidget.ui \
    src/config/decisions/buy/buydecision2configwidget.ui \
    src/config/decisions/buy/buydecision3configwidget.ui \
    src/config/decisions/decisionmakerconfigwidget.ui \
    src/config/decisions/sell/selldecision1configwidget.ui \
    src/config/decisions/sell/selldecision2configwidget.ui \
    src/config/decisions/sell/selldecision3configwidget.ui \
    src/config/settingsdialog.ui \
    src/main/mainwindow.ui
