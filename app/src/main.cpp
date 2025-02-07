#include <QApplication>

#include <QDebug>
#include <QMessageBox>
#include <QSystemTrayIcon>
#include <QTranslator>

#include "src/config/config.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h"
#include "src/config/decisions/decisionmakerconfig.h"
#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1config.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2config.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidgetfactory.h"
#include "src/config/settingsdialog/settingsdialogfactory.h"
#include "src/config/settingseditor/settingseditor.h"
#include "src/db/stocks/stocksdatabase.h"
#include "src/logger/logger.h"
#include "src/main/mainwindow.h"
#include "src/widgets/trayicon/trayiconfactory.h"
#include "src/storage/stocks/stocksstorage.h"
#include "src/threads/cleanup/cleanupthread.h"
#include "src/threads/makedecision/makedecisionthread.h"

#ifdef QT_NO_SYSTEMTRAYICON
#error "QSystemTrayIcon is not supported on this platform"
#endif



int runApplication(int argc, char *argv[]);



int main(int argc, char *argv[])
{
    Logger::init();

    qInfo() << "START";

    int res = runApplication(argc, argv);

    qInfo() << "END";

    return res;
}

int runApplication(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Q_INIT_RESOURCE(Resources);

    QTranslator translator;
    if (!translator.load(":/assets/translations/language_ru.qm"))
    {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Localization"),
            QObject::tr("Failed to apply localization"),
            QMessageBox::Close
        );

        return 1;
    }

    app.installTranslator(&translator);
    qDebug() << "Localization applied";

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(
            nullptr,
            QObject::tr("Systray"),
            QObject::tr("Failed to detect any system tray on this system"),
            QMessageBox::Close
        );

        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    BuyDecision1Config simulatorBuyDecision1Config;
    BuyDecision2Config simulatorBuyDecision2Config;
    BuyDecision3Config simulatorBuyDecision3Config;
    SellDecision1Config simulatorSellDecision1Config;
    SellDecision2Config simulatorSellDecision2Config;
    SellDecision3Config simulatorSellDecision3Config;

    BuyDecision1Config autoPilotBuyDecision1Config;
    BuyDecision2Config autoPilotBuyDecision2Config;
    BuyDecision3Config autoPilotBuyDecision3Config;
    SellDecision1Config autoPilotSellDecision1Config;
    SellDecision2Config autoPilotSellDecision2Config;
    SellDecision3Config autoPilotSellDecision3Config;

    BuyDecision1Config simulatorBuyDecision1ConfigForSettingsDialog;
    BuyDecision2Config simulatorBuyDecision2ConfigForSettingsDialog;
    BuyDecision3Config simulatorBuyDecision3ConfigForSettingsDialog;
    SellDecision1Config simulatorSellDecision1ConfigForSettingsDialog;
    SellDecision2Config simulatorSellDecision2ConfigForSettingsDialog;
    SellDecision3Config simulatorSellDecision3ConfigForSettingsDialog;

    BuyDecision1Config autoPilotBuyDecision1ConfigForSettingsDialog;
    BuyDecision2Config autoPilotBuyDecision2ConfigForSettingsDialog;
    BuyDecision3Config autoPilotBuyDecision3ConfigForSettingsDialog;
    SellDecision1Config autoPilotSellDecision1ConfigForSettingsDialog;
    SellDecision2Config autoPilotSellDecision2ConfigForSettingsDialog;
    SellDecision3Config autoPilotSellDecision3ConfigForSettingsDialog;

    DecisionMakerConfig simulatorConfig(
        &simulatorBuyDecision1Config,
        &simulatorBuyDecision2Config,
        &simulatorBuyDecision3Config,
        &simulatorSellDecision1Config,
        &simulatorSellDecision2Config,
        &simulatorSellDecision3Config
    );
    DecisionMakerConfig autoPilotConfig(
        &autoPilotBuyDecision1Config,
        &autoPilotBuyDecision2Config,
        &autoPilotBuyDecision3Config,
        &autoPilotSellDecision1Config,
        &autoPilotSellDecision2Config,
        &autoPilotSellDecision3Config
    );
    DecisionMakerConfig simulatorConfigForSettingsDialog(
        &simulatorBuyDecision1ConfigForSettingsDialog,
        &simulatorBuyDecision2ConfigForSettingsDialog,
        &simulatorBuyDecision3ConfigForSettingsDialog,
        &simulatorSellDecision1ConfigForSettingsDialog,
        &simulatorSellDecision2ConfigForSettingsDialog,
        &simulatorSellDecision3ConfigForSettingsDialog
    );
    DecisionMakerConfig autoPilotConfigForSettingsDialog(
        &autoPilotBuyDecision1ConfigForSettingsDialog,
        &autoPilotBuyDecision2ConfigForSettingsDialog,
        &autoPilotBuyDecision3ConfigForSettingsDialog,
        &autoPilotSellDecision1ConfigForSettingsDialog,
        &autoPilotSellDecision2ConfigForSettingsDialog,
        &autoPilotSellDecision3ConfigForSettingsDialog
    );

    Config config(&simulatorConfig, &autoPilotConfig);
    Config configForSettingsDialog(&simulatorConfigForSettingsDialog, &autoPilotConfigForSettingsDialog);

    SettingsDialogFactory settingsDialogFactory;

    DecisionMakerConfigWidgetFactory decisionMakerConfigWidgetFactory;
    BuyDecision1ConfigWidgetFactory  buyDecision1ConfigWidgetFactory;
    BuyDecision2ConfigWidgetFactory  buyDecision2ConfigWidgetFactory;
    BuyDecision3ConfigWidgetFactory  buyDecision3ConfigWidgetFactory;
    SellDecision1ConfigWidgetFactory sellDecision1ConfigWidgetFactory;
    SellDecision2ConfigWidgetFactory sellDecision2ConfigWidgetFactory;
    SellDecision3ConfigWidgetFactory sellDecision3ConfigWidgetFactory;

    TrayIconFactory trayIconFactory;

    SettingsEditor settingsEditor("GrisCom", "TInvestor");

    StocksDatabase stocksDatabase;
    StocksStorage stocksStorage;
    CleanupThread cleanupThread(&config, &stocksDatabase, &stocksStorage);
    MakeDecisionThread makeDecisionThread(&config, &stocksDatabase, &stocksStorage);

    MainWindow mainWindow(
        &config,
        &configForSettingsDialog,
        &settingsDialogFactory,
        &decisionMakerConfigWidgetFactory,
        &buyDecision1ConfigWidgetFactory,
        &buyDecision2ConfigWidgetFactory,
        &buyDecision3ConfigWidgetFactory,
        &sellDecision1ConfigWidgetFactory,
        &sellDecision2ConfigWidgetFactory,
        &sellDecision3ConfigWidgetFactory,
        &trayIconFactory,
        &settingsEditor,
        &stocksDatabase,
        &stocksStorage,
        &cleanupThread,
        &makeDecisionThread
    );
    mainWindow.init();

    return app.exec();
}
