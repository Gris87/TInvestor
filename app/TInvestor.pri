VPATH = $$PWD
INCLUDEPATH += $$PWD



# find app/ -name \*.cpp | grep -ve test_ -ve app/src/main.cpp | sed "s/app\///g" | sed "s/\.cpp/\.cpp \\\/g"
SOURCES += \
    src/config/config.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1config.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.cpp \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2config.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.cpp \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3config.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.cpp \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.cpp \
    src/config/decisions/decisionmakerconfig.cpp \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.cpp \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1config.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.cpp \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2config.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.cpp \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3config.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.cpp \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidgetfactory.cpp \
    src/config/settingsdialog/settingsdialog.cpp \
    src/config/settingsdialog/settingsdialogfactory.cpp \
    src/config/settingseditor/settingseditor.cpp \
    src/db/stocks/stocksdatabase.cpp \
    src/logger/logger.cpp \
    src/main/mainwindow.cpp \
    src/storage/stocks/stocksstorage.cpp \
    src/threads/cleanup/cleanupthread.cpp \
    src/threads/refresh/refreshthread.cpp \
    src/widgets/trayicon/trayicon.cpp \
    src/widgets/trayicon/trayiconfactory.cpp

# find app/ -name \*.h | grep -ve _mock.h | sed "s/app\///g" | sed "s/\.h/\.h \\\/g"
HEADERS += \
    src/config/config.h \
    src/config/decisions/buy/buydecision1config/buydecision1config.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget.h \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory.h \
    src/config/decisions/buy/buydecision1config/ibuydecision1config.h \
    src/config/decisions/buy/buydecision2config/buydecision2config.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget.h \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory.h \
    src/config/decisions/buy/buydecision2config/ibuydecision2config.h \
    src/config/decisions/buy/buydecision3config/buydecision3config.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget.h \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory.h \
    src/config/decisions/buy/buydecision3config/ibuydecision3config.h \
    src/config/decisions/decisionmakerconfig.h \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.h \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.h \
    src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget.h \
    src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h \
    src/config/decisions/idecisionmakerconfig.h \
    src/config/decisions/sell/selldecision1config/iselldecision1config.h \
    src/config/decisions/sell/selldecision1config/selldecision1config.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.h \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.h \
    src/config/decisions/sell/selldecision2config/iselldecision2config.h \
    src/config/decisions/sell/selldecision2config/selldecision2config.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.h \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h \
    src/config/decisions/sell/selldecision3config/iselldecision3config.h \
    src/config/decisions/sell/selldecision3config/selldecision3config.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.h \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidgetfactory.h \
    src/config/iconfig.h \
    src/config/settingsdialog/isettingsdialog.h \
    src/config/settingsdialog/isettingsdialogfactory.h \
    src/config/settingsdialog/settingsdialog.h \
    src/config/settingsdialog/settingsdialogfactory.h \
    src/config/settingseditor/isettingseditor.h \
    src/config/settingseditor/settingseditor.h \
    src/db/stocks/istocksdatabase.h \
    src/db/stocks/stocksdatabase.h \
    src/domain/stocks/stock.h \
    src/logger/logger.h \
    src/main/mainwindow.h \
    src/storage/stocks/istocksstorage.h \
    src/storage/stocks/stocksstorage.h \
    src/threads/cleanup/cleanupthread.h \
    src/threads/cleanup/icleanupthread.h \
    src/threads/parallelhelper/parallelhelperthread.h \
    src/threads/refresh/irefreshthread.h \
    src/threads/refresh/refreshthread.h \
    src/widgets/trayicon/itrayicon.h \
    src/widgets/trayicon/itrayiconfactory.h \
    src/widgets/trayicon/trayicon.h \
    src/widgets/trayicon/trayiconfactory.h

# find app/ -name \*.ui | sed "s/app\///g" | sed "s/\.ui/\.ui \\\/g"
FORMS += \
    src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.ui \
    src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.ui \
    src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.ui \
    src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.ui \
    src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidget.ui \
    src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.ui \
    src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.ui \
    src/config/settingsdialog/settingsdialog.ui \
    src/main/mainwindow.ui
