VPATH = $$PWD
INCLUDEPATH += $$PWD



SOURCES += \
    src/config/config.cpp \
    src/config/decisions/decisionmakerconfig.cpp \
    src/config/decisions/decisionmakerconfigwidget.cpp \
    src/config/decisions/buy/buydecision1config.cpp \
    src/config/decisions/buy/buydecision2config.cpp \
    src/config/decisions/buy/buydecision3config.cpp \
    src/config/decisions/buy/buydecision1configwidget.cpp \
    src/config/decisions/buy/buydecision2configwidget.cpp \
    src/config/decisions/buy/buydecision3configwidget.cpp \
    src/config/decisions/sell/selldecision1config.cpp \
    src/config/decisions/sell/selldecision2config.cpp \
    src/config/decisions/sell/selldecision3config.cpp \
    src/config/decisions/sell/selldecision1configwidget.cpp \
    src/config/decisions/sell/selldecision2configwidget.cpp \
    src/config/decisions/sell/selldecision3configwidget.cpp \
    src/config/settingsdialog.cpp \
    src/db/stocks/stocksdatabase.cpp \
    src/logger/logger.cpp \
    src/main/mainwindow.cpp \
    src/main/trayicon.cpp \
    src/threads/cleanupthread.cpp \
    src/threads/refreshthread.cpp

HEADERS += \
    src/config/config.h \
    src/config/decisions/decisionmakerconfig.h \
    src/config/decisions/decisionmakerconfigwidget.h \
    src/config/decisions/idecisionmakerconfig.h \
    src/config/decisions/buy/buydecision1config.h \
    src/config/decisions/buy/buydecision2config.h \
    src/config/decisions/buy/buydecision3config.h \
    src/config/decisions/buy/buydecision1configwidget.h \
    src/config/decisions/buy/buydecision2configwidget.h \
    src/config/decisions/buy/buydecision3configwidget.h \
    src/config/decisions/sell/selldecision1config.h \
    src/config/decisions/sell/selldecision2config.h \
    src/config/decisions/sell/selldecision3config.h \
    src/config/decisions/sell/selldecision1configwidget.h \
    src/config/decisions/sell/selldecision2configwidget.h \
    src/config/decisions/sell/selldecision3configwidget.h \
    src/config/settingsdialog.h \
    src/db/stocks/stocksdatabase.h \
    src/domain/stocks/stock.h \
    src/logger/logger.h \
    src/main/mainwindow.h \
    src/main/trayicon.h \
    src/threads/cleanupthread.h \
    src/threads/refreshthread.h

FORMS += \
    src/config/decisions/decisionmakerconfigwidget.ui \
    src/config/decisions/buy/buydecision1configwidget.ui \
    src/config/decisions/buy/buydecision2configwidget.ui \
    src/config/decisions/buy/buydecision3configwidget.ui \
    src/config/decisions/sell/selldecision1configwidget.ui \
    src/config/decisions/sell/selldecision2configwidget.ui \
    src/config/decisions/sell/selldecision3configwidget.ui \
    src/config/settingsdialog.ui \
    src/main/mainwindow.ui
