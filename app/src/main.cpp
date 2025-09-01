#include <QApplication>

#include <QDebug>
#include <QMessageBox>
#include <QStyleFactory>
#include <QSystemTrayIcon>
#include <QTextStream>
#include <QTranslator>

#include "src/config/config.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1config.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2config.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3config.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4config.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidgetfactory.h"
#include "src/config/decisions/decisionmakerconfig.h"
#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1config.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2config.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3config.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidgetfactory.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4config.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidgetfactory.h"
#include "src/db/instruments/instrumentsdatabase.h"
#include "src/db/logos/logosdatabase.h"
#include "src/db/logs/logsdatabase.h"
#include "src/db/operations/operationsdatabase.h"
#include "src/db/portfolio/portfoliodatabase.h"
#include "src/db/stocks/stocksdatabase.h"
#include "src/db/user/userdatabase.h"
#include "src/decisions/buy/buydecision1/buydecision1.h"
#include "src/decisions/buy/buydecision2/buydecision2.h"
#include "src/decisions/buy/buydecision3/buydecision3.h"
#include "src/decisions/buy/buydecision4/buydecision4.h"
#include "src/decisions/decisionmaker.h"
#include "src/decisions/sell/selldecision1/selldecision1.h"
#include "src/decisions/sell/selldecision2/selldecision2.h"
#include "src/decisions/sell/selldecision3/selldecision3.h"
#include "src/decisions/sell/selldecision4/selldecision4.h"
#include "src/dialogs/authdialog/authdialogfactory.h"
#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"
#include "src/dialogs/settingsdialog/settingsdialogfactory.h"
#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"
#include "src/dialogs/startsimulationdialog/startsimulationdialogfactory.h"
#include "src/grpc/grpcclient.h"
#include "src/grpc/rawgrpcclient.h"
#include "src/main/mainwindow.h"
#include "src/storage/instruments/instrumentsstorage.h"
#include "src/storage/logos/logosstorage.h"
#include "src/storage/stocks/stocksstorage.h"
#include "src/storage/user/userstorage.h"
#include "src/threads/autopilotdecisionmaker/autopilotdecisionmakerthread.h"
#include "src/threads/cleanup/cleanupthread.h"
#include "src/threads/follow/followthread.h"
#include "src/threads/lastprice//lastpricethread.h"
#include "src/threads/logs/logsthread.h"
#include "src/threads/operations/operationsthread.h"
#include "src/threads/orderbook/orderbookthread.h"
#include "src/threads/portfolio/portfoliothread.h"
#include "src/threads/portfoliolastprice/portfoliolastpricethread.h"
#include "src/threads/pricecollect/pricecollectthread.h"
#include "src/threads/simulatordaterangedecisionmaker/simulatordaterangedecisionmakerthread.h"
#include "src/threads/simulatordecisionmaker/simulatordecisionmakerthread.h"
#include "src/threads/trading/tradingthreadfactory.h"
#include "src/threads/userupdate/userupdatethread.h"
#include "src/utils/autorunenabler/autorunenabler.h"
#include "src/utils/filedialog/filedialogfactory.h"
#include "src/utils/fs/dir/dirfactory.h"
#include "src/utils/fs/file/filefactory.h"
#include "src/utils/fs/zip/qzip/qzipfactory.h"
#include "src/utils/fs/zip/qzipfile/qzipfilefactory.h"
#include "src/utils/http/httpclient.h"
#include "src/utils/logger/logger.h"
#include "src/utils/messagebox/messageboxutils.h"
#include "src/utils/optimizer/optimizer.h"
#include "src/utils/settingseditor/settingseditor.h"
#include "src/utils/style/darkpalette.h"
#include "src/utils/timeutils/timeutils.h"
#include "src/utils/tradeutils/tradeutils.h"
#include "src/widgets/accountchartwidget/accountchartwidgetfactory.h"
#include "src/widgets/actionstableitemwidget/actionstableitemwidgetfactory.h"
#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"
#include "src/widgets/logsfilterwidget/logsfilterwidgetfactory.h"
#include "src/widgets/logstablewidget/logstablewidgetfactory.h"
#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"
#include "src/widgets/orderwaveswidget/orderwaveswidgetfactory.h"
#include "src/widgets/portfoliotreewidget/portfoliotreewidgetfactory.h"
#include "src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h"
#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"
#include "src/widgets/tablemodels/logstablemodel/logstablemodelfactory.h"
#include "src/widgets/tablemodels/operationstablemodel/operationstablemodelfactory.h"
#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodelfactory.h"
#include "src/widgets/tablemodels/stockstablemodel/stockstablemodelfactory.h"
#include "src/widgets/trayicon/trayiconfactory.h"

#ifdef QT_NO_SYSTEMTRAYICON
#error "QSystemTrayIcon is not supported on this platform"
#endif



const char* const VERSION  = "v1.0.1";
const char* const GIT_HASH = "DEV_BUILD";

constexpr int    DETECT_TRAY_ATTEMPTS = 5;
constexpr qint64 MS_IN_SECOND         = 1000LL;
constexpr qint64 DETECT_TRAY_DELAY    = MS_IN_SECOND; // 1 second



static bool isHelpNeeded(int argc, char* argv[])
{
    const QStringList helpArguments{"--help", "-h", "-?"};

    for (int i = 0; i < argc; ++i)
    {
        if (helpArguments.contains(argv[i]))
        {
            return true;
        }
    }

    return false;
}

static bool isVersionNeeded(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        if (QString(argv[i]) == "--version")
        {
            return true;
        }
    }

    return false;
}

static bool isLogToFileNeeded(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        if (QString(argv[i]) == "--log-to-file")
        {
            return true;
        }
    }

    return false;
}

static int runApplication(QApplication* app)
{
    app->setStyle(QStyleFactory::create("Fusion"));
    app->setPalette(DarkPalette());

    Q_INIT_RESOURCE(Resources);

    QTranslator translator;
    if (!translator.load(":/assets/translations/language_ru.qm"))
    {
        QMessageBox::critical(
            nullptr, QObject::tr("Localization"), QObject::tr("Failed to apply localization"), QMessageBox::Close
        );

        return 1;
    }

    app->installTranslator(&translator);
    qDebug() << "Localization applied";

    bool trayFound = false;

    for (int i = 0; i < DETECT_TRAY_ATTEMPTS; ++i)
    {
        if (QSystemTrayIcon::isSystemTrayAvailable())
        {
            trayFound = true;

            break;
        }

        QThread::msleep(DETECT_TRAY_DELAY);
    }

    if (!trayFound)
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
    BuyDecision4ConfigWidgetFactory  buyDecision4ConfigWidgetFactory;
    SellDecision1ConfigWidgetFactory sellDecision1ConfigWidgetFactory;
    SellDecision2ConfigWidgetFactory sellDecision2ConfigWidgetFactory;
    SellDecision3ConfigWidgetFactory sellDecision3ConfigWidgetFactory;
    SellDecision4ConfigWidgetFactory sellDecision4ConfigWidgetFactory;
    ActionsTableItemWidgetFactory    actionsTableItemWidgetFactory;
    OrderWavesWidgetFactory          orderWavesWidgetFactory;
    StocksControlsWidgetFactory      stocksControlsWidgetFactory;
    StocksTableWidgetFactory         stocksTableWidgetFactory;
    OperationsTableWidgetFactory     operationsTableWidgetFactory;
    AccountChartWidgetFactory        accountChartWidgetFactory;
    LogsFilterWidgetFactory          logsFilterWidgetFactory;
    LogsTableWidgetFactory           logsTableWidgetFactory;
    PortfolioTreeWidgetFactory       portfolioTreeWidgetFactory;
    DecisionMakerWidgetFactory       decisionMakerWidgetFactory;

    StocksTableModelFactory     stocksTableModelFactory;
    OperationsTableModelFactory operationsTableModelFactory;
    LogsTableModelFactory       logsTableModelFactory;
    PortfolioTreeModelFactory   portfolioTreeModelFactory;

    TrayIconFactory trayIconFactory;
    DirFactory      dirFactory;
    FileFactory     fileFactory;
    QZipFactory     qZipFactory;
    QZipFileFactory qZipFileFactory;

    SettingsEditor settingsEditor("GrisCom", "TInvestor");
    SettingsEditor simulatorSettingsEditor(app->applicationDirPath() + "/data/simulator/config.ini", QSettings::IniFormat);
    SettingsEditor autoPilotSettingsEditor(app->applicationDirPath() + "/data/autopilot/config.ini", QSettings::IniFormat);
    SettingsEditor autorunSettingsEditor("Microsoft", "Windows");
    AutorunEnabler autorunEnabler(&autorunSettingsEditor, &dirFactory, &fileFactory);

    BuyDecision1Config  simulatorBuyDecision1Config;
    BuyDecision2Config  simulatorBuyDecision2Config;
    BuyDecision3Config  simulatorBuyDecision3Config;
    BuyDecision4Config  simulatorBuyDecision4Config;
    SellDecision1Config simulatorSellDecision1Config;
    SellDecision2Config simulatorSellDecision2Config;
    SellDecision3Config simulatorSellDecision3Config;
    SellDecision4Config simulatorSellDecision4Config;

    BuyDecision1Config  autoPilotBuyDecision1Config;
    BuyDecision2Config  autoPilotBuyDecision2Config;
    BuyDecision3Config  autoPilotBuyDecision3Config;
    BuyDecision4Config  autoPilotBuyDecision4Config;
    SellDecision1Config autoPilotSellDecision1Config;
    SellDecision2Config autoPilotSellDecision2Config;
    SellDecision3Config autoPilotSellDecision3Config;
    SellDecision4Config autoPilotSellDecision4Config;

    BuyDecision1Config  simulatorBuyDecision1ConfigForSettingsDialog;
    BuyDecision2Config  simulatorBuyDecision2ConfigForSettingsDialog;
    BuyDecision3Config  simulatorBuyDecision3ConfigForSettingsDialog;
    BuyDecision4Config  simulatorBuyDecision4ConfigForSettingsDialog;
    SellDecision1Config simulatorSellDecision1ConfigForSettingsDialog;
    SellDecision2Config simulatorSellDecision2ConfigForSettingsDialog;
    SellDecision3Config simulatorSellDecision3ConfigForSettingsDialog;
    SellDecision4Config simulatorSellDecision4ConfigForSettingsDialog;

    BuyDecision1Config  autoPilotBuyDecision1ConfigForSettingsDialog;
    BuyDecision2Config  autoPilotBuyDecision2ConfigForSettingsDialog;
    BuyDecision3Config  autoPilotBuyDecision3ConfigForSettingsDialog;
    BuyDecision4Config  autoPilotBuyDecision4ConfigForSettingsDialog;
    SellDecision1Config autoPilotSellDecision1ConfigForSettingsDialog;
    SellDecision2Config autoPilotSellDecision2ConfigForSettingsDialog;
    SellDecision3Config autoPilotSellDecision3ConfigForSettingsDialog;
    SellDecision4Config autoPilotSellDecision4ConfigForSettingsDialog;

    BuyDecision1Config  simulatorBuyDecision1ConfigForSimulation;
    BuyDecision2Config  simulatorBuyDecision2ConfigForSimulation;
    BuyDecision3Config  simulatorBuyDecision3ConfigForSimulation;
    BuyDecision4Config  simulatorBuyDecision4ConfigForSimulation;
    SellDecision1Config simulatorSellDecision1ConfigForSimulation;
    SellDecision2Config simulatorSellDecision2ConfigForSimulation;
    SellDecision3Config simulatorSellDecision3ConfigForSimulation;
    SellDecision4Config simulatorSellDecision4ConfigForSimulation;

    BuyDecision1Config  autoPilotBuyDecision1ConfigForSimulation;
    BuyDecision2Config  autoPilotBuyDecision2ConfigForSimulation;
    BuyDecision3Config  autoPilotBuyDecision3ConfigForSimulation;
    BuyDecision4Config  autoPilotBuyDecision4ConfigForSimulation;
    SellDecision1Config autoPilotSellDecision1ConfigForSimulation;
    SellDecision2Config autoPilotSellDecision2ConfigForSimulation;
    SellDecision3Config autoPilotSellDecision3ConfigForSimulation;
    SellDecision4Config autoPilotSellDecision4ConfigForSimulation;

    DecisionMakerConfig simulatorConfig(
        &simulatorBuyDecision1Config,
        &simulatorBuyDecision2Config,
        &simulatorBuyDecision3Config,
        &simulatorBuyDecision4Config,
        &simulatorSellDecision1Config,
        &simulatorSellDecision2Config,
        &simulatorSellDecision3Config,
        &simulatorSellDecision4Config
    );
    DecisionMakerConfig autoPilotConfig(
        &autoPilotBuyDecision1Config,
        &autoPilotBuyDecision2Config,
        &autoPilotBuyDecision3Config,
        &autoPilotBuyDecision4Config,
        &autoPilotSellDecision1Config,
        &autoPilotSellDecision2Config,
        &autoPilotSellDecision3Config,
        &autoPilotSellDecision4Config
    );
    DecisionMakerConfig simulatorConfigForSettingsDialog(
        &simulatorBuyDecision1ConfigForSettingsDialog,
        &simulatorBuyDecision2ConfigForSettingsDialog,
        &simulatorBuyDecision3ConfigForSettingsDialog,
        &simulatorBuyDecision4ConfigForSettingsDialog,
        &simulatorSellDecision1ConfigForSettingsDialog,
        &simulatorSellDecision2ConfigForSettingsDialog,
        &simulatorSellDecision3ConfigForSettingsDialog,
        &simulatorSellDecision4ConfigForSettingsDialog
    );
    DecisionMakerConfig autoPilotConfigForSettingsDialog(
        &autoPilotBuyDecision1ConfigForSettingsDialog,
        &autoPilotBuyDecision2ConfigForSettingsDialog,
        &autoPilotBuyDecision3ConfigForSettingsDialog,
        &autoPilotBuyDecision4ConfigForSettingsDialog,
        &autoPilotSellDecision1ConfigForSettingsDialog,
        &autoPilotSellDecision2ConfigForSettingsDialog,
        &autoPilotSellDecision3ConfigForSettingsDialog,
        &autoPilotSellDecision4ConfigForSettingsDialog
    );
    DecisionMakerConfig simulatorConfigForSimulation(
        &simulatorBuyDecision1ConfigForSimulation,
        &simulatorBuyDecision2ConfigForSimulation,
        &simulatorBuyDecision3ConfigForSimulation,
        &simulatorBuyDecision4ConfigForSimulation,
        &simulatorSellDecision1ConfigForSimulation,
        &simulatorSellDecision2ConfigForSimulation,
        &simulatorSellDecision3ConfigForSimulation,
        &simulatorSellDecision4ConfigForSimulation
    );
    DecisionMakerConfig autoPilotConfigForSimulation(
        &autoPilotBuyDecision1ConfigForSimulation,
        &autoPilotBuyDecision2ConfigForSimulation,
        &autoPilotBuyDecision3ConfigForSimulation,
        &autoPilotBuyDecision4ConfigForSimulation,
        &autoPilotSellDecision1ConfigForSimulation,
        &autoPilotSellDecision2ConfigForSimulation,
        &autoPilotSellDecision3ConfigForSimulation,
        &autoPilotSellDecision4ConfigForSimulation
    );

    Config config(&simulatorConfig, &autoPilotConfig);
    Config configForSettingsDialog(&simulatorConfigForSettingsDialog, &autoPilotConfigForSettingsDialog);
    Config configForSimulation(&simulatorConfigForSimulation, &autoPilotConfigForSimulation);

    UserDatabase        userDatabase;
    UserStorage         userStorage(&userDatabase);
    InstrumentsDatabase instrumentsDatabase(&dirFactory, &fileFactory);
    InstrumentsStorage  instrumentsStorage(&instrumentsDatabase);
    LogosDatabase       logosDatabase(&dirFactory, &fileFactory);
    LogosStorage        logosStorage(&logosDatabase);
    StocksDatabase      stocksDatabase(&dirFactory, &fileFactory, &logosStorage);
    StocksStorage       stocksStorage(&stocksDatabase, &userStorage);
    OperationsDatabase  simulatorOperationsDatabase(&dirFactory, &fileFactory, &logosStorage, false);
    LogsDatabase        simulatorLogsDatabase(&dirFactory, &fileFactory, &logosStorage, false);
    PortfolioDatabase   simulatorPortfolioDatabase(&dirFactory, &fileFactory, &logosStorage);
    OperationsDatabase  autoPilotOperationsDatabase(&dirFactory, &fileFactory, &logosStorage, true);
    LogsDatabase        autoPilotLogsDatabase(&dirFactory, &fileFactory, &logosStorage, true);

    TimeUtils         timeUtils;
    TradeUtils        tradeUtils;
    FileDialogFactory fileDialogFactory;
    MessageBoxUtils   messageBoxUtils;
    HttpClient        httpClient;
    RawGrpcClient     rawGrpcClient;
    GrpcClient        grpcClient(&userStorage, &rawGrpcClient, &timeUtils);
    Optimizer         optimizer;

    BuyDecision1  buyDecision1;
    BuyDecision2  buyDecision2;
    BuyDecision3  buyDecision3;
    BuyDecision4  buyDecision4;
    SellDecision1 sellDecision1;
    SellDecision2 sellDecision2;
    SellDecision3 sellDecision3;
    SellDecision4 sellDecision4;

    const QList<IActionDecision*> buyDecisions  = {&buyDecision1, &buyDecision2, &buyDecision3, &buyDecision4};
    const QList<IActionDecision*> sellDecisions = {&sellDecision1, &sellDecision2, &sellDecision3, &sellDecision4};

    DecisionMaker simulatorRealtimeDecisionMaker(
        &instrumentsStorage, &userStorage, &timeUtils, &tradeUtils, buyDecisions, sellDecisions
    );
    DecisionMaker simulatorDateRangeDecisionMaker(
        &instrumentsStorage, &userStorage, &timeUtils, &tradeUtils, buyDecisions, sellDecisions
    );
    DecisionMaker autoPilotRealtimeDecisionMaker(
        &instrumentsStorage, &userStorage, &timeUtils, &tradeUtils, buyDecisions, sellDecisions
    );

    CleanupThread      cleanupThread(&config, &stocksStorage);
    UserUpdateThread   userUpdateThread(&userStorage, &grpcClient);
    PriceCollectThread priceCollectThread(
        &config,
        &userStorage,
        &stocksStorage,
        &instrumentsStorage,
        &logosStorage,
        &dirFactory,
        &fileFactory,
        &qZipFactory,
        &qZipFileFactory,
        &timeUtils,
        &httpClient,
        &grpcClient
    );
    LastPriceThread          lastPriceThread(&stocksStorage, &timeUtils, &grpcClient);
    PortfolioLastPriceThread simulatorPortfolioLastPriceThread(&timeUtils, &grpcClient);
    OperationsThread         operationsThread(
        &autoPilotOperationsDatabase, &instrumentsStorage, &logosStorage, &timeUtils, &grpcClient, &optimizer
    );
    LogsThread                   logsThread(&autoPilotLogsDatabase, &instrumentsStorage, &logosStorage, &optimizer);
    PortfolioThread              portfolioThread(&instrumentsStorage, &logosStorage, &timeUtils, &grpcClient);
    PortfolioLastPriceThread     autoPilotPortfolioLastPriceThread(&timeUtils, &grpcClient);
    SimulatorDecisionMakerThread simulatorDecisionMakerThread(
        &simulatorSettingsEditor,
        &simulatorOperationsDatabase,
        &simulatorLogsDatabase,
        &simulatorPortfolioDatabase,
        &instrumentsStorage,
        &logosStorage,
        &userStorage,
        &stocksStorage,
        &config,
        &simulatorRealtimeDecisionMaker,
        &optimizer
    );
    SimulatorDateRangeDecisionMakerThread simulatorDateRangeDecisionMakerThread(
        &dirFactory,
        &fileFactory,
        &simulatorSettingsEditor,
        &simulatorOperationsDatabase,
        &simulatorLogsDatabase,
        &simulatorPortfolioDatabase,
        &instrumentsStorage,
        &logosStorage,
        &userStorage,
        &stocksStorage,
        &configForSimulation,
        &simulatorDateRangeDecisionMaker,
        &optimizer
    );
    AutoPilotDecisionMakerThread autoPilotDecisionMakerThread(
        &stocksStorage, &config, &autoPilotRealtimeDecisionMaker, &grpcClient
    );
    FollowThread         followThread(&instrumentsStorage, &grpcClient);
    OrderBookThread      orderBookThread(&grpcClient);
    TradingThreadFactory tradingThreadFactory;

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
        &buyDecision4ConfigWidgetFactory,
        &sellDecision1ConfigWidgetFactory,
        &sellDecision2ConfigWidgetFactory,
        &sellDecision3ConfigWidgetFactory,
        &sellDecision4ConfigWidgetFactory,
        &actionsTableItemWidgetFactory,
        &orderWavesWidgetFactory,
        &stocksControlsWidgetFactory,
        &stocksTableWidgetFactory,
        &operationsTableWidgetFactory,
        &accountChartWidgetFactory,
        &logsFilterWidgetFactory,
        &logsTableWidgetFactory,
        &portfolioTreeWidgetFactory,
        &stocksTableModelFactory,
        &operationsTableModelFactory,
        &logsTableModelFactory,
        &portfolioTreeModelFactory,
        &decisionMakerWidgetFactory,
        &trayIconFactory,
        &userStorage,
        &stocksStorage,
        &instrumentsStorage,
        &logosStorage,
        &httpClient,
        &grpcClient,
        &cleanupThread,
        &userUpdateThread,
        &priceCollectThread,
        &lastPriceThread,
        &simulatorPortfolioLastPriceThread,
        &operationsThread,
        &logsThread,
        &portfolioThread,
        &autoPilotPortfolioLastPriceThread,
        &simulatorDecisionMakerThread,
        &simulatorDateRangeDecisionMakerThread,
        &autoPilotDecisionMakerThread,
        &followThread,
        &orderBookThread,
        &tradingThreadFactory,
        &fileDialogFactory,
        &timeUtils,
        &messageBoxUtils,
        &settingsEditor,
        &simulatorSettingsEditor,
        &autoPilotSettingsEditor,
        &autorunEnabler
    );
    mainWindow.init();

    qInfo() << "UP and Running";

    if (!app->arguments().contains("--autorun"))
    {
        mainWindow.show();
    }

    return app->exec();
}

int main(int argc, char* argv[])
{
    if (isHelpNeeded(argc, argv))
    {
        QTextStream out(stdout);

        out << "Usage:\n";
        out << "  TInvestor [OPTIONS]\n";
        out << "\n";
        out << "Options:\n";
        out << "  -h            - Display this message\n";
        out << "  --help        - Display this message\n";
        out << "  -?            - Display this message\n";
        out << "  --version     - Prints version info\n";
        out << "  --log-to-file - Write logs to logs.txt file\n";
        out << "  --autorun     - Run application in hidden mode\n";

        return 0;
    }

    if (isVersionNeeded(argc, argv))
    {
        QTextStream out(stdout);

        out << VERSION << " - " << GIT_HASH << Qt::endl;

        return 0;
    }

    Logger::init();

    if (isLogToFileNeeded(argc, argv))
    {
        Logger::enableLogToFile();
    }

    QApplication app(argc, argv);

    qInfo() << "START";

    const int res = runApplication(&app);

    qInfo() << "END";

    return res;
}
