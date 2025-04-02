#include <QApplication>

#include <QDebug>
#include <QMessageBox>
#include <QStyleFactory>
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
#include "src/db/stocks/stocksdatabase.h"
#include "src/db/user/userdatabase.h"
#include "src/dialogs/authdialog/authdialogfactory.h"
#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"
#include "src/dialogs/settingsdialog/settingsdialogfactory.h"
#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"
#include "src/dialogs/startsimulationdialog/startsimulationdialogfactory.h"
#include "src/grpc/grpcclient.h"
#include "src/grpc/rawgrpcclient.h"
#include "src/main/mainwindow.h"
#include "src/storage/stocks/stocksstorage.h"
#include "src/storage/user/userstorage.h"
#include "src/threads/cleanup/cleanupthread.h"
#include "src/threads/lastprice//lastpricethread.h"
#include "src/threads/makedecision/makedecisionthread.h"
#include "src/threads/orderbook/orderbookthread.h"
#include "src/threads/pricecollect/pricecollectthread.h"
#include "src/threads/userupdate/userupdatethread.h"
#include "src/utils/fs/dir/dirfactory.h"
#include "src/utils/fs/file/filefactory.h"
#include "src/utils/fs/zip/qzip/qzipfactory.h"
#include "src/utils/fs/zip/qzipfile/qzipfilefactory.h"
#include "src/utils/http/httpclient.h"
#include "src/utils/logger/logger.h"
#include "src/utils/messagebox/messagebox.h"
#include "src/utils/settingseditor/settingseditor.h"
#include "src/utils/style/darkpalette.h"
#include "src/utils/timeutils/timeutils.h"
#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"
#include "src/widgets/orderwaveswidget/orderwaveswidgetfactory.h"
#include "src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h"
#include "src/widgets/tablerecord/items/actions/actionstableitemwidgetfactory.h"
#include "src/widgets/tablerecord/items/stock/stocktableitemwidgetfactory.h"
#include "src/widgets/tablerecord/tablerecordfactory.h"
#include "src/widgets/trayicon/trayiconfactory.h"

#ifdef QT_NO_SYSTEMTRAYICON
#error "QSystemTrayIcon is not supported on this platform"
#endif



int runApplication(int argc, char* argv[]);



int main(int argc, char* argv[])
{
    Logger::init();

    qInfo() << "START";

    int res = runApplication(argc, argv);

    qInfo() << "END";

    return res;
}

int runApplication(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    app.setPalette(DarkPalette());

    Q_INIT_RESOURCE(Resources);

    QTranslator translator;
    if (!translator.load(":/assets/translations/language_ru.qm"))
    {
        QMessageBox::critical(
            nullptr, QObject::tr("Localization"), QObject::tr("Failed to apply localization"), QMessageBox::Close
        );

        return 1;
    }

    app.installTranslator(&translator);
    qDebug() << "Localization applied";

    if (!QSystemTrayIcon::isSystemTrayAvailable())
    {
        QMessageBox::critical(
            nullptr, QObject::tr("Systray"), QObject::tr("Failed to detect any system tray on this system"), QMessageBox::Close
        );

        return 1;
    }

    QApplication::setQuitOnLastWindowClosed(false);

    AuthDialogFactory            authDialogFactory;
    SettingsDialogFactory        settingsDialogFactory;
    OrderWavesDialogFactory      orderWavesDialogFactory;
    StartSimulationDialogFactory startSimulationDialogFactory;
    StartAutoPilotDialogFactory  startAutoPilotDialogFactory;

    DecisionMakerConfigWidgetFactory decisionMakerConfigWidgetFactory;
    BuyDecision1ConfigWidgetFactory  buyDecision1ConfigWidgetFactory;
    BuyDecision2ConfigWidgetFactory  buyDecision2ConfigWidgetFactory;
    BuyDecision3ConfigWidgetFactory  buyDecision3ConfigWidgetFactory;
    SellDecision1ConfigWidgetFactory sellDecision1ConfigWidgetFactory;
    SellDecision2ConfigWidgetFactory sellDecision2ConfigWidgetFactory;
    SellDecision3ConfigWidgetFactory sellDecision3ConfigWidgetFactory;
    StockTableItemWidgetFactory      stockTableItemWidgetFactory;
    ActionsTableItemWidgetFactory    actionsTableItemWidgetFactory;
    OrderWavesWidgetFactory          orderWavesWidgetFactory;
    StocksControlsWidgetFactory      stocksControlsWidgetFactory;
    DecisionMakerWidgetFactory       decisionMakerWidgetFactory;

    TableRecordFactory tableRecordFactory;
    TrayIconFactory    trayIconFactory;
    DirFactory         dirFactory;
    FileFactory        fileFactory;
    QZipFactory        qZipFactory;
    QZipFileFactory    qZipFileFactory;

    SettingsEditor settingsEditor("GrisCom", "TInvestor");
    SettingsEditor autorunSettingsEditor("Microsoft", "Windows");

    BuyDecision1Config  simulatorBuyDecision1Config;
    BuyDecision2Config  simulatorBuyDecision2Config;
    BuyDecision3Config  simulatorBuyDecision3Config;
    SellDecision1Config simulatorSellDecision1Config;
    SellDecision2Config simulatorSellDecision2Config;
    SellDecision3Config simulatorSellDecision3Config;

    BuyDecision1Config  autoPilotBuyDecision1Config;
    BuyDecision2Config  autoPilotBuyDecision2Config;
    BuyDecision3Config  autoPilotBuyDecision3Config;
    SellDecision1Config autoPilotSellDecision1Config;
    SellDecision2Config autoPilotSellDecision2Config;
    SellDecision3Config autoPilotSellDecision3Config;

    BuyDecision1Config  simulatorBuyDecision1ConfigForSettingsDialog;
    BuyDecision2Config  simulatorBuyDecision2ConfigForSettingsDialog;
    BuyDecision3Config  simulatorBuyDecision3ConfigForSettingsDialog;
    SellDecision1Config simulatorSellDecision1ConfigForSettingsDialog;
    SellDecision2Config simulatorSellDecision2ConfigForSettingsDialog;
    SellDecision3Config simulatorSellDecision3ConfigForSettingsDialog;

    BuyDecision1Config  autoPilotBuyDecision1ConfigForSettingsDialog;
    BuyDecision2Config  autoPilotBuyDecision2ConfigForSettingsDialog;
    BuyDecision3Config  autoPilotBuyDecision3ConfigForSettingsDialog;
    SellDecision1Config autoPilotSellDecision1ConfigForSettingsDialog;
    SellDecision2Config autoPilotSellDecision2ConfigForSettingsDialog;
    SellDecision3Config autoPilotSellDecision3ConfigForSettingsDialog;

    BuyDecision1Config  simulatorBuyDecision1ConfigForSimulation;
    BuyDecision2Config  simulatorBuyDecision2ConfigForSimulation;
    BuyDecision3Config  simulatorBuyDecision3ConfigForSimulation;
    SellDecision1Config simulatorSellDecision1ConfigForSimulation;
    SellDecision2Config simulatorSellDecision2ConfigForSimulation;
    SellDecision3Config simulatorSellDecision3ConfigForSimulation;

    BuyDecision1Config  autoPilotBuyDecision1ConfigForSimulation;
    BuyDecision2Config  autoPilotBuyDecision2ConfigForSimulation;
    BuyDecision3Config  autoPilotBuyDecision3ConfigForSimulation;
    SellDecision1Config autoPilotSellDecision1ConfigForSimulation;
    SellDecision2Config autoPilotSellDecision2ConfigForSimulation;
    SellDecision3Config autoPilotSellDecision3ConfigForSimulation;

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
    DecisionMakerConfig simulatorConfigForSimulation(
        &simulatorBuyDecision1ConfigForSimulation,
        &simulatorBuyDecision2ConfigForSimulation,
        &simulatorBuyDecision3ConfigForSimulation,
        &simulatorSellDecision1ConfigForSimulation,
        &simulatorSellDecision2ConfigForSimulation,
        &simulatorSellDecision3ConfigForSimulation
    );
    DecisionMakerConfig autoPilotConfigForSimulation(
        &autoPilotBuyDecision1ConfigForSimulation,
        &autoPilotBuyDecision2ConfigForSimulation,
        &autoPilotBuyDecision3ConfigForSimulation,
        &autoPilotSellDecision1ConfigForSimulation,
        &autoPilotSellDecision2ConfigForSimulation,
        &autoPilotSellDecision3ConfigForSimulation
    );

    Config config(&simulatorConfig, &autoPilotConfig);
    Config configForSettingsDialog(&simulatorConfigForSettingsDialog, &autoPilotConfigForSettingsDialog);
    Config configForSimulation(&simulatorConfigForSimulation, &autoPilotConfigForSimulation);

    UserDatabase   userDatabase;
    UserStorage    userStorage(&userDatabase);
    StocksDatabase stocksDatabase(&dirFactory, &fileFactory);
    StocksStorage  stocksStorage(&stocksDatabase, &userStorage);

    TimeUtils       timeUtils;
    MessageBoxUtils messageBoxUtils;
    HttpClient      httpClient;
    RawGrpcClient   rawGrpcClient;
    GrpcClient      grpcClient(&userStorage, &rawGrpcClient, &timeUtils);

    UserUpdateThread   userUpdateThread(&userStorage, &grpcClient);
    PriceCollectThread priceCollectThread(
        &config,
        &userStorage,
        &stocksStorage,
        &dirFactory,
        &fileFactory,
        &qZipFactory,
        &qZipFileFactory,
        &timeUtils,
        &httpClient,
        &grpcClient
    );
    LastPriceThread    lastPriceThread(&stocksStorage, &timeUtils, &grpcClient);
    CleanupThread      cleanupThread(&config, &stocksStorage);
    MakeDecisionThread makeDecisionThread(&config, &stocksDatabase, &stocksStorage);
    OrderBookThread    orderBookThread(&grpcClient);

    MainWindow mainWindow(
        &config,
        &configForSettingsDialog,
        &configForSimulation,
        &authDialogFactory,
        &settingsDialogFactory,
        &orderWavesDialogFactory,
        &startSimulationDialogFactory,
        &startAutoPilotDialogFactory,
        &decisionMakerConfigWidgetFactory,
        &buyDecision1ConfigWidgetFactory,
        &buyDecision2ConfigWidgetFactory,
        &buyDecision3ConfigWidgetFactory,
        &sellDecision1ConfigWidgetFactory,
        &sellDecision2ConfigWidgetFactory,
        &sellDecision3ConfigWidgetFactory,
        &stockTableItemWidgetFactory,
        &actionsTableItemWidgetFactory,
        &orderWavesWidgetFactory,
        &tableRecordFactory,
        &stocksControlsWidgetFactory,
        &decisionMakerWidgetFactory,
        &trayIconFactory,
        &userStorage,
        &stocksStorage,
        &httpClient,
        &grpcClient,
        &userUpdateThread,
        &priceCollectThread,
        &lastPriceThread,
        &cleanupThread,
        &makeDecisionThread,
        &orderBookThread,
        &messageBoxUtils,
        &settingsEditor,
        &autorunSettingsEditor
    );
    mainWindow.init();

    qInfo() << "UP and Running";

    if (!qApp->arguments().contains("--autorun"))
    {
        mainWindow.show();
    }

    return app.exec();
}
