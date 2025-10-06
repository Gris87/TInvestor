#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>
#include <QRandomGenerator>

#include "src/threads/parallelhelper/parallelhelperthread.h"
#include "src/widgets/floatinglabel/floatinglabel.h"



const QColor      GREY_COLOR                               = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static
const QColor      NORMAL_COLOR                             = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static
const char* const DATE_FORMAT                              = "yyyy-MM-dd";
const char* const DATETIME_FORMAT                          = "yyyy-MM-dd hh:mm:ss";
constexpr int     SMALL_SPINNER_INNER_RADIUS               = 6;
constexpr int     SMALL_SPINNER_LINE_LENGTH                = 6;
constexpr qint64  MS_IN_SECOND                             = 1000LL;
constexpr qint64  ONE_MINUTE                               = 60LL * MS_IN_SECOND;
constexpr qint64  ONE_HOUR                                 = 60LL * ONE_MINUTE;
constexpr qint64  ONE_DAY                                  = 24LL * ONE_HOUR;
constexpr qint64  USER_UPDATE_INTERVAL                     = 15LL * ONE_MINUTE; // 15 minutes
constexpr qint64  PRICE_COLLECT_INTERVAL                   = ONE_HOUR;          // 1 hour
constexpr qint64  CLEANUP_INTERVAL                         = ONE_DAY;           // 1 day
constexpr qint64  STOCKS_TABLE_UPDATE_ALL_INTERVAL         = ONE_DAY;           // 1 day
constexpr qint64  STOCKS_TABLE_UPDATE_LAST_PRICES_INTERVAL = 3 * MS_IN_SECOND;  // 3 seconds
constexpr qint64  PORTFOLIO_UPDATE_LAST_PRICES_INTERVAL    = 3 * MS_IN_SECOND;  // 3 seconds

#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



MainWindow::MainWindow(
    IConfig*                                config,
    IConfig*                                configForSettingsDialog,
    IConfig*                                configForSimulation,
    IAuthDialogFactory*                     authDialogFactory,
    ISettingsDialogFactory*                 settingsDialogFactory,
    IOrderWavesDialogFactory*               orderWavesDialogFactory,
    IStartSimulationDialogFactory*          startSimulationDialogFactory,
    IStartAutoPilotDialogFactory*           startAutoPilotDialogFactory,
    IDecisionMakerConfigWidgetFactory*      decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*       buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*       buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*       buyDecision3ConfigWidgetFactory,
    IBuyDecision4ConfigWidgetFactory*       buyDecision4ConfigWidgetFactory,
    IBuyDecision5ConfigWidgetFactory*       buyDecision5ConfigWidgetFactory,
    IBuyDecision6ConfigWidgetFactory*       buyDecision6ConfigWidgetFactory,
    IBuyDecision7ConfigWidgetFactory*       buyDecision7ConfigWidgetFactory,
    IBuyDecision8ConfigWidgetFactory*       buyDecision8ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory*      sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory*      sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory*      sellDecision3ConfigWidgetFactory,
    ISellDecision4ConfigWidgetFactory*      sellDecision4ConfigWidgetFactory,
    ISellDecision5ConfigWidgetFactory*      sellDecision5ConfigWidgetFactory,
    IActionsTableItemWidgetFactory*         actionsTableItemWidgetFactory,
    IOrderWavesWidgetFactory*               orderWavesWidgetFactory,
    IStocksControlsWidgetFactory*           stocksControlsWidgetFactory,
    IStocksTableWidgetFactory*              stocksTableWidgetFactory,
    IOperationsTableWidgetFactory*          operationsTableWidgetFactory,
    IAccountChartWidgetFactory*             accountChartWidgetFactory,
    ILogsFilterWidgetFactory*               logsFilterWidgetFactory,
    ILogsTableWidgetFactory*                logsTableWidgetFactory,
    IPortfolioTreeWidgetFactory*            portfolioTreeWidgetFactory,
    IStocksTableModelFactory*               stocksTableModelFactory,
    IOperationsTableModelFactory*           operationsTableModelFactory,
    ILogsTableModelFactory*                 logsTableModelFactory,
    IPortfolioTreeModelFactory*             portfolioTreeModelFactory,
    IDecisionMakerWidgetFactory*            decisionMakerWidgetFactory,
    ITrayIconFactory*                       trayIconFactory,
    IUserStorage*                           userStorage,
    IStocksStorage*                         stocksStorage,
    IInstrumentsStorage*                    instrumentsStorage,
    ILogosStorage*                          logosStorage,
    IHttpClient*                            httpClient,
    IGrpcClient*                            grpcClient,
    IGrpcRetryClient*                       grpcRetryClient,
    ICleanupThread*                         cleanupThread,
    IUserUpdateThread*                      userUpdateThread,
    IPriceCollectThread*                    priceCollectThread,
    ILastPriceThread*                       lastPriceThread,
    IPortfolioLastPriceThread*              simulatorPortfolioLastPriceThread,
    IOperationsThread*                      operationsThread,
    ILogsThread*                            logsThread,
    IPortfolioThread*                       portfolioThread,
    IPortfolioLastPriceThread*              autoPilotPortfolioLastPriceThread,
    ISimulatorDecisionMakerThread*          simulatorDecisionMakerThread,
    ISimulatorDateRangeDecisionMakerThread* simulatorDateRangeDecisionMakerThread,
    IAutoPilotDecisionMakerThread*          autoPilotDecisionMakerThread,
    IBiDirTradingControlThread*             biDirTradingControlThread,
    IHighLiquidityThread*                   highLiquidityThread,
    IFollowThread*                          followThread,
    IOrderBookThread*                       orderBookThread,
    ITradingThreadFactory*                  tradingThreadFactory,
    IBiDirTradingThreadFactory*             biDirTradingThreadFactory,
    IFileDialogFactory*                     fileDialogFactory,
    ITimeUtils*                             timeUtils,
    ITradeUtils*                            tradeUtils,
    IMessageBoxUtils*                       messageBoxUtils,
    ISettingsEditor*                        settingsEditor,
    ISettingsEditor*                        simulatorSettingsEditor,
    ISettingsEditor*                        autoPilotSettingsEditor,
    IAutorunEnabler*                        autorunEnabler,
    QWidget*                                parent
) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    authFailedDialogShown(false),
    tradingThreads(),
    biDirTradingThreads(),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mConfigForSimulation(configForSimulation),
    mAuthDialogFactory(authDialogFactory),
    mSettingsDialogFactory(settingsDialogFactory),
    mStartSimulationDialogFactory(startSimulationDialogFactory),
    mStartAutoPilotDialogFactory(startAutoPilotDialogFactory),
    mDecisionMakerConfigWidgetFactory(decisionMakerConfigWidgetFactory),
    mBuyDecision1ConfigWidgetFactory(buyDecision1ConfigWidgetFactory),
    mBuyDecision2ConfigWidgetFactory(buyDecision2ConfigWidgetFactory),
    mBuyDecision3ConfigWidgetFactory(buyDecision3ConfigWidgetFactory),
    mBuyDecision4ConfigWidgetFactory(buyDecision4ConfigWidgetFactory),
    mBuyDecision5ConfigWidgetFactory(buyDecision5ConfigWidgetFactory),
    mBuyDecision6ConfigWidgetFactory(buyDecision6ConfigWidgetFactory),
    mBuyDecision7ConfigWidgetFactory(buyDecision7ConfigWidgetFactory),
    mBuyDecision8ConfigWidgetFactory(buyDecision8ConfigWidgetFactory),
    mSellDecision1ConfigWidgetFactory(sellDecision1ConfigWidgetFactory),
    mSellDecision2ConfigWidgetFactory(sellDecision2ConfigWidgetFactory),
    mSellDecision3ConfigWidgetFactory(sellDecision3ConfigWidgetFactory),
    mSellDecision4ConfigWidgetFactory(sellDecision4ConfigWidgetFactory),
    mSellDecision5ConfigWidgetFactory(sellDecision5ConfigWidgetFactory),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mInstrumentsStorage(instrumentsStorage),
    mLogosStorage(logosStorage),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mGrpcRetryClient(grpcRetryClient),
    mCleanupThread(cleanupThread),
    mUserUpdateThread(userUpdateThread),
    mPriceCollectThread(priceCollectThread),
    mLastPriceThread(lastPriceThread),
    mSimulatorPortfolioLastPriceThread(simulatorPortfolioLastPriceThread),
    mOperationsThread(operationsThread),
    mLogsThread(logsThread),
    mPortfolioThread(portfolioThread),
    mAutoPilotPortfolioLastPriceThread(autoPilotPortfolioLastPriceThread),
    mSimulatorDecisionMakerThread(simulatorDecisionMakerThread),
    mSimulatorDateRangeDecisionMakerThread(simulatorDateRangeDecisionMakerThread),
    mAutoPilotDecisionMakerThread(autoPilotDecisionMakerThread),
    mBiDirTradingControlThread(biDirTradingControlThread),
    mHighLiquidityThread(highLiquidityThread),
    mFollowThread(followThread),
    mOrderBookThread(orderBookThread),
    mTradingThreadFactory(tradingThreadFactory),
    mBiDirTradingThreadFactory(biDirTradingThreadFactory),
    mFileDialogFactory(fileDialogFactory),
    mTimeUtils(timeUtils),
    mTradeUtils(tradeUtils),
    mMessageBoxUtils(messageBoxUtils),
    mSettingsEditor(settingsEditor),
    mSimulatorSettingsEditor(simulatorSettingsEditor),
    mAutoPilotSettingsEditor(autoPilotSettingsEditor),
    mAutorunEnabler(autorunEnabler),
    mAutoPilotAccountId(),
    mAutoPilotAnotherAccountId()
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    removeToolBar(ui->toolBar);
    addToolBar(Qt::RightToolBarArea, ui->toolBar);
    ui->toolBar->show();

    ui->waitingSpinnerWidget->setColor(GREY_COLOR);
    ui->waitingSpinnerWidget->setTextColor(GREY_COLOR);

    ui->simulationActiveWidget->hide();
    ui->simulationActiveSpinnerWidget->setInnerRadius(SMALL_SPINNER_INNER_RADIUS);
    ui->simulationActiveSpinnerWidget->setLineLength(SMALL_SPINNER_LINE_LENGTH);
    ui->simulationActiveSpinnerWidget->setColor(GREY_COLOR);

    ui->autoPilotActiveWidget->hide();
    ui->autoPilotActiveSpinnerWidget->setInnerRadius(SMALL_SPINNER_INNER_RADIUS);
    ui->autoPilotActiveSpinnerWidget->setLineLength(SMALL_SPINNER_LINE_LENGTH);
    ui->autoPilotActiveSpinnerWidget->setColor(GREY_COLOR);

    mStocksControlsWidget = stocksControlsWidgetFactory->newInstance(mSettingsEditor, this);
    mStocksTableWidget    = stocksTableWidgetFactory->newInstance(
        stocksTableModelFactory,
        actionsTableItemWidgetFactory,
        orderWavesDialogFactory,
        orderWavesWidgetFactory,
        mUserStorage,
        mOrderBookThread,
        mHttpClient,
        mFileDialogFactory,
        mSettingsEditor,
        this
    );
    mSimulatorDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(
        DecisionMakerWidgetFactoryNewInstanceArgsMore15(
            operationsTableWidgetFactory,
            accountChartWidgetFactory,
            logsFilterWidgetFactory,
            logsTableWidgetFactory,
            decisionMakerConfigWidgetFactory,
            buyDecision1ConfigWidgetFactory,
            buyDecision2ConfigWidgetFactory,
            buyDecision3ConfigWidgetFactory,
            buyDecision4ConfigWidgetFactory,
            buyDecision5ConfigWidgetFactory,
            buyDecision6ConfigWidgetFactory,
            buyDecision7ConfigWidgetFactory,
            buyDecision8ConfigWidgetFactory,
            sellDecision1ConfigWidgetFactory,
            sellDecision2ConfigWidgetFactory,
            sellDecision3ConfigWidgetFactory,
            sellDecision4ConfigWidgetFactory,
            sellDecision5ConfigWidgetFactory,
            portfolioTreeWidgetFactory,
            operationsTableModelFactory,
            logsTableModelFactory,
            portfolioTreeModelFactory,
            mFileDialogFactory,
            config,
            configForSimulation,
            mSettingsEditor
        ),
        this
    );
    mAutoPilotDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(
        DecisionMakerWidgetFactoryNewInstanceArgsMore15(
            operationsTableWidgetFactory,
            accountChartWidgetFactory,
            logsFilterWidgetFactory,
            logsTableWidgetFactory,
            decisionMakerConfigWidgetFactory,
            buyDecision1ConfigWidgetFactory,
            buyDecision2ConfigWidgetFactory,
            buyDecision3ConfigWidgetFactory,
            buyDecision4ConfigWidgetFactory,
            buyDecision5ConfigWidgetFactory,
            buyDecision6ConfigWidgetFactory,
            buyDecision7ConfigWidgetFactory,
            buyDecision8ConfigWidgetFactory,
            sellDecision1ConfigWidgetFactory,
            sellDecision2ConfigWidgetFactory,
            sellDecision3ConfigWidgetFactory,
            sellDecision4ConfigWidgetFactory,
            sellDecision5ConfigWidgetFactory,
            portfolioTreeWidgetFactory,
            operationsTableModelFactory,
            logsTableModelFactory,
            portfolioTreeModelFactory,
            mFileDialogFactory,
            config,
            configForSimulation,
            mSettingsEditor
        ),
        this
    );

    mSimulatorDecisionMakerWidget->setAccountName(tr("Simulator"));
    mAutoPilotDecisionMakerWidget->setAccountName(tr("Auto-pilot"));

    mStocksTableWidget->setFilter(mStocksControlsWidget->getFilter());

    ui->layoutForStocksControlsWidget->addWidget(mStocksControlsWidget);
    ui->layoutForStocksTableWidget->addWidget(mStocksTableWidget);
    ui->layoutForSimulatorDecisionMaker->addWidget(mSimulatorDecisionMakerWidget);
    ui->layoutForAutoPilotDecisionMaker->addWidget(mAutoPilotDecisionMakerWidget);

    mTrayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(mTrayIcon,                                SIGNAL(activated(QSystemTrayIcon::ActivationReason)),        this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon,                                SIGNAL(trayIconShowClicked()),                               this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon,                                SIGNAL(trayIconExitClicked()),                               this, SLOT(trayIconExitClicked()));
    connect(mGrpcClient,                              SIGNAL(authFailed(const QString&)),                          this, SLOT(authFailed(const QString&)));
    connect(&cleanupTimer,                            SIGNAL(timeout()),                                           this, SLOT(cleanupTimerTicked()));
    connect(&userUpdateTimer,                         SIGNAL(timeout()),                                           this, SLOT(userUpdateTimerTicked()));
    connect(&priceCollectTimer,                       SIGNAL(timeout()),                                           this, SLOT(priceCollectTimerTicked()));
    connect(&makeDecisionTimer,                       SIGNAL(timeout()),                                           this, SLOT(makeDecisionTimerTicked()));
    connect(&stocksTableUpdateAllTimer,               SIGNAL(timeout()),                                           this, SLOT(stocksTableUpdateAllTimerTicked()));
    connect(&stocksTableUpdateLastPricesTimer,        SIGNAL(timeout()),                                           this, SLOT(stocksTableUpdateLastPricesTimerTicked()));
    connect(&simulatorPortfolioUpdateLastPricesTimer, SIGNAL(timeout()),                                           this, SLOT(simulatorPortfolioUpdateLastPricesTimerTicked()));
    connect(&autoPilotPortfolioUpdateLastPricesTimer, SIGNAL(timeout()),                                           this, SLOT(autoPilotPortfolioUpdateLastPricesTimerTicked()));
    connect(mPriceCollectThread,                      SIGNAL(notifyInstrumentsProgress(const QString&)),           this, SLOT(notifyInstrumentsProgress(const QString&)));
    connect(mPriceCollectThread,                      SIGNAL(stocksChanged()),                                     this, SLOT(stocksChanged()));
    connect(mPriceCollectThread,                      SIGNAL(pricesChanged()),                                     this, SLOT(pricesChanged()));
    connect(mPriceCollectThread,                      SIGNAL(periodicDataChanged()),                               this, SLOT(periodicDataChanged()));
    connect(mLastPriceThread,                         SIGNAL(lastPriceChanged(const QString&)),                    this, SLOT(lastPriceChanged(const QString&)));
    connect(mSimulatorPortfolioLastPriceThread,       SIGNAL(lastPriceChanged(const QString&, float)),             this, SLOT(simulatorPortfolioLastPriceChanged(const QString&, float)));
    connect(mOperationsThread,                        SIGNAL(operationsRead(const QList<Operation>&)),             this, SLOT(autoPilotOperationsRead(const QList<Operation>&)));
    connect(mOperationsThread,                        SIGNAL(operationsAdded(const QList<Operation>&)),            this, SLOT(autoPilotOperationsAdded(const QList<Operation>&)));
    connect(mLogsThread,                              SIGNAL(logsRead(const QList<LogEntry>&)),                    this, SLOT(autoPilotLogsRead(const QList<LogEntry>&)));
    connect(mLogsThread,                              SIGNAL(logAdded(const LogEntry&)),                           this, SLOT(autoPilotLogAdded(const LogEntry&)));
    connect(mPortfolioThread,                         SIGNAL(portfolioChanged(const Portfolio&)),                  this, SLOT(autoPilotPortfolioChanged(const Portfolio&)));
    connect(mAutoPilotPortfolioLastPriceThread,       SIGNAL(lastPriceChanged(const QString&, float)),             this, SLOT(autoPilotPortfolioLastPriceChanged(const QString&, float)));
    connect(mSimulatorDecisionMakerThread,            SIGNAL(operationsRead(const QList<Operation>&)),             this, SLOT(simulatorOperationsRead(const QList<Operation>&)));
    connect(mSimulatorDecisionMakerThread,            SIGNAL(operationsAdded(const QList<Operation>&)),            this, SLOT(simulatorOperationsAdded(const QList<Operation>&)));
    connect(mSimulatorDecisionMakerThread,            SIGNAL(logsRead(const QList<LogEntry>&)),                    this, SLOT(simulatorLogsRead(const QList<LogEntry>&)));
    connect(mSimulatorDecisionMakerThread,            SIGNAL(logAdded(const LogEntry&)),                           this, SLOT(simulatorLogAdded(const LogEntry&)));
    connect(mSimulatorDecisionMakerThread,            SIGNAL(portfolioChanged(const Portfolio&)),                  this, SLOT(simulatorPortfolioChanged(const Portfolio&)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(stepProgressChanged(int, int)),                       this, SLOT(simulatorStepProgressChanged(int, int)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(totalProgressChanged(int, int)),                      this, SLOT(simulatorTotalProgressChanged(int, int)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(progressChanged(int, int, const QString&)),           this, SLOT(simulatorProgressChanged(int, int, const QString&)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(resultFound(const QString&, const QColor&)),          this, SLOT(simulatorResultFound(const QString&, const QColor&)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(bestResultChanged(const QString&, const QColor&)),    this, SLOT(simulatorBestResultChanged(const QString&, const QColor&)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(bestConfigFound()),                                   this, SLOT(simulatorBestConfigFound()));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(operationsRead(const QList<Operation>&)),             this, SLOT(simulatorOperationsRead(const QList<Operation>&)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(logsRead(const QList<LogEntry>&)),                    this, SLOT(simulatorLogsRead(const QList<LogEntry>&)));
    connect(mSimulatorDateRangeDecisionMakerThread,   SIGNAL(portfolioChanged(const Portfolio&)),                  this, SLOT(simulatorPortfolioChanged(const Portfolio&)));
    connect(mAutoPilotDecisionMakerThread,            SIGNAL(tradeInstruments(const InstrumentsForTrading&)),      this, SLOT(autoPilotTradeInstruments(const InstrumentsForTrading&)));
    connect(mBiDirTradingControlThread,               SIGNAL(tradeInstruments(const InstrumentsForBiDirTrading&)), this, SLOT(autoPilotBiDirTradeInstruments(const InstrumentsForBiDirTrading&)));
    connect(mHighLiquidityThread,                     SIGNAL(tradeInstruments(const InstrumentsForTrading&)),      this, SLOT(autoPilotTradeInstruments(const InstrumentsForTrading&)));
    connect(mFollowThread,                            SIGNAL(tradeInstruments(const InstrumentsForTrading&)),      this, SLOT(autoPilotTradeInstruments(const InstrumentsForTrading&)));
    connect(mStocksControlsWidget,                    SIGNAL(dateChangeDateTimeChanged(const QDateTime&)),         this, SLOT(dateChangeDateTimeChanged(const QDateTime&)));
    connect(mStocksControlsWidget,                    SIGNAL(filterChanged(const StockFilter&)),                   this, SLOT(stockFilterChanged(const StockFilter&)));
    // clang-format on

    mTrayIcon->show();

    mConfig->makeDefault();
    mConfig->load(mSettingsEditor);
    applyConfig();

    loadWindowState();
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroy MainWindow";

    mCleanupThread->terminateThread();
    mUserUpdateThread->terminateThread();
    mPriceCollectThread->terminateThread();
    mLastPriceThread->terminateThread();
    mSimulatorPortfolioLastPriceThread->terminateThread();
    mOperationsThread->terminateThread();
    mLogsThread->terminateThread();
    mPortfolioThread->terminateThread();
    mAutoPilotPortfolioLastPriceThread->terminateThread();
    mSimulatorDecisionMakerThread->terminateThread();
    mSimulatorDateRangeDecisionMakerThread->terminateThread();
    mAutoPilotDecisionMakerThread->terminateThread();
    mBiDirTradingControlThread->terminateThread();
    mHighLiquidityThread->terminateThread();
    mFollowThread->terminateThread();

    for (auto it = tradingThreads.constBegin(); it != tradingThreads.constEnd(); ++it)
    {
        it.value()->terminateThread();
    }

    for (auto it = biDirTradingThreads.constBegin(); it != biDirTradingThreads.constEnd(); ++it)
    {
        it.value()->terminateThread();
    }

    mCleanupThread->wait();
    mUserUpdateThread->wait();
    mPriceCollectThread->wait();
    mLastPriceThread->wait();
    mSimulatorPortfolioLastPriceThread->wait();
    mOperationsThread->wait();
    mLogsThread->wait();
    mPortfolioThread->wait();
    mAutoPilotPortfolioLastPriceThread->wait();
    mSimulatorDecisionMakerThread->wait();
    mSimulatorDateRangeDecisionMakerThread->wait();
    mAutoPilotDecisionMakerThread->wait();
    mBiDirTradingControlThread->wait();
    mHighLiquidityThread->wait();
    mFollowThread->wait();

    for (auto it = tradingThreads.constBegin(); it != tradingThreads.constEnd(); ++it)
    {
        it.value()->wait();
        delete it.value();
    }

    for (auto it = biDirTradingThreads.constBegin(); it != biDirTradingThreads.constEnd(); ++it)
    {
        it.value()->wait();
        delete it.value();
    }

    saveWindowState();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (!event->spontaneous() || !isVisible())
    {
        qDebug() << "Application closed";

        return;
    }

    event->ignore();
    hide();

    const QString title = tr("TInvestor");
    const QString msg   = tr("TInvestor is working in background");

#ifdef Q_OS_WINDOWS
    mTrayIcon->showMessage(title, msg);
#else
    Q_UNUSED(title);
    Q_UNUSED(msg);
#endif

    qDebug() << "Main window moved to tray";
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == DOUBLE_CLICK_REASON)
    {
        qDebug() << "Tray icon double clicked";

        trayIconShowClicked();
    }
}

void MainWindow::trayIconShowClicked()
{
    show();
    raise();
    activateWindow();

    qDebug() << "Main window displayed";
}

void MainWindow::trayIconExitClicked()
{
    QCoreApplication::quit();
}

void MainWindow::authFailed(const QString& errorCodeString)
{
    if (authFailedDialogShown)
    {
        return;
    }

    mLogsThread->addLog(LOG_LEVEL_ERROR, "", tr("GRPC error happened with code %1").arg(errorCodeString));

    mUserUpdateThread->terminateThread();
    mPriceCollectThread->terminateThread();
    mLastPriceThread->terminateThread();

    userUpdateTimer.stop();
    priceCollectTimer.stop();
    makeDecisionTimer.stop();
    stocksTableUpdateAllTimer.stop();
    stocksTableUpdateLastPricesTimer.stop();

    stopSimulator();
    stopAutoPilot();

    mUserUpdateThread->wait();
    mPriceCollectThread->wait();
    mLastPriceThread->wait();

    ui->actionAuth->setEnabled(true);
    ui->waitingSpinnerWidget->setText(tr("Waiting for authorization"));
    trayIconShowClicked();

    authFailedDialogShown = true;

    const std::shared_ptr<IAuthDialog> dialog = mAuthDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, this);

    if (dialog->exec() == QDialog::Accepted)
    {
        mUserStorage->writeLock();
        mUserStorage->setToken(dialog->getToken());
        mUserStorage->writeUnlock();

        on_actionAuth_triggered();
    }

    authFailedDialogShown = false;
}

void MainWindow::cleanupTimerTicked()
{
    qInfo() << "Cleanup timer ticked";

    mCleanupThread->start();
}

void MainWindow::userUpdateTimerTicked()
{
    qDebug() << "User update timer ticked";

    mUserUpdateThread->start();
}

void MainWindow::priceCollectTimerTicked()
{
    qDebug() << "Price collect timer ticked";

    mPriceCollectThread->start();
}

void MainWindow::makeDecisionTimerTicked()
{
    qDebug() << "Make decision timer ticked";

    if (ui->simulationActiveSpinnerWidget->isSpinning())
    {
        const QString mode = mSimulatorSettingsEditor->value("Options/Mode", SIMULATOR_MODE_REALTIME).toString();

        if (mode == SIMULATOR_MODE_REALTIME)
        {
            mSimulatorDecisionMakerThread->start();
        }
    }

    if (ui->autoPilotActiveSpinnerWidget->isSpinning())
    {
        const QString mode = mAutoPilotSettingsEditor->value("Options/Mode", AUTO_PILOT_MODE_VIEW).toString();

        if (mode == AUTO_PILOT_MODE_INTERNAL)
        {
            mAutoPilotDecisionMakerThread->start();

            if (mConfig->isTradeHugeSpread())
            {
                mBiDirTradingControlThread->start();
            }

            if (mConfig->isTradeLiquidityEtfNightly())
            {
                mHighLiquidityThread->start();
            }
        }
    }
}

void MainWindow::stocksTableUpdateAllTimerTicked()
{
    qDebug() << "Stocks table update all timer ticked";

    mStocksTableWidget->updateAll();
}

void MainWindow::stocksTableUpdateLastPricesTimerTicked()
{
    qDebug() << "Stocks table update timer ticked";

    mStocksTableWidget->updateLastPrices();
}

void MainWindow::simulatorPortfolioUpdateLastPricesTimerTicked()
{
    qDebug() << "Simulator portfolio update timer ticked";

    mSimulatorDecisionMakerWidget->updateLastPrices();
}

void MainWindow::autoPilotPortfolioUpdateLastPricesTimerTicked()
{
    qDebug() << "Auto-pilot portfolio update timer ticked";

    mAutoPilotDecisionMakerWidget->updateLastPrices();
}

void MainWindow::notifyInstrumentsProgress(const QString& message) const
{
    ui->waitingSpinnerWidget->setText(message);
}

void MainWindow::stocksChanged()
{
    qInfo() << "Stocks changed";

    updateStocksTableWidget();
    mLastPriceThread->stocksChanged();
}

void MainWindow::pricesChanged()
{
    mStocksTableWidget->updatePrices();
}

void MainWindow::periodicDataChanged()
{
    mStocksTableWidget->updatePeriodicData();
}

void MainWindow::lastPriceChanged(const QString& instrumentId)
{
    mStocksTableWidget->lastPriceChanged(instrumentId);
}

void MainWindow::dateChangeDateTimeChanged(const QDateTime& dateTime)
{
    mStocksStorage->readLock();
    mStocksStorage->obtainStocksDatePrice(dateTime.toMSecsSinceEpoch());
    mStocksStorage->readUnlock();

    mStocksTableWidget->setDateChangeTooltip(tr("From: %1").arg(dateTime.toString(DATETIME_FORMAT)));
    mStocksTableWidget->updatePrices();
}

void MainWindow::stockFilterChanged(const StockFilter& filter)
{
    mStocksTableWidget->setFilter(filter);
}

void MainWindow::startSimulator()
{
    const QString mode = mSimulatorSettingsEditor->value("Options/Mode", SIMULATOR_MODE_REALTIME).toString();

    ui->simulationActiveWidget->show();
    ui->simulationActiveSpinnerWidget->start();

    ui->startSimulationButton->setIcon(QIcon(":/assets/images/stop.png"));
    ui->startSimulationButton->setText(tr("Stop simulation"));

    mSimulatorPortfolioLastPriceThread->start();

    if (mode == SIMULATOR_MODE_REALTIME)
    {
        ui->simulatorWaitingStackedWidget->setCurrentWidget(ui->simulatorWorkingPage);

        if (!mSimulatorDecisionMakerThread->isLoaded())
        {
            mSimulatorDecisionMakerWidget->showSpinners();
        }

        mSimulatorDecisionMakerThread->start();
    }
    else if (mode == SIMULATOR_MODE_DATERANGE)
    {
        const QString fromDate   = mSimulatorSettingsEditor->value("Options/FromDate", "").toString();
        const QString toDate     = mSimulatorSettingsEditor->value("Options/ToDate", "").toString();
        const bool    bestConfig = mSimulatorSettingsEditor->value("Options/BestConfig", false).toBool();

        ui->simulatorWaitingStackedWidget->setCurrentWidget(ui->simulatorWaitingPage);
        ui->simulatorStepProgressBar->setVisible(bestConfig);
        ui->simulatorTotalProgressBar->setVisible(bestConfig);
        ui->simulatorBestResultWidget->setVisible(bestConfig);
        ui->simulatorDateRangeLabel->setText(fromDate + " - " + toDate);
        ui->simulatorRemainingTimeLabel->setText("00:00:00");
        ui->simulatorStepProgressBar->setValue(0);
        ui->simulatorTotalProgressBar->setValue(0);
        ui->simulatorProgressBar->setValue(0);
        simulatorBestResultChanged("0.00%", NORMAL_COLOR);

        mConfigForSimulation->assign(mConfig);
        mSimulatorDateRangeDecisionMakerThread->start();
    }

    simulatorPortfolioUpdateLastPricesTimer.start();
}

void MainWindow::stopSimulator()
{
    ui->simulationActiveWidget->hide();
    ui->simulationActiveSpinnerWidget->stop();

    ui->startSimulationButton->setIcon(QIcon(":/assets/images/start.png"));
    ui->startSimulationButton->setText(tr("Start simulation"));

    mSimulatorPortfolioLastPriceThread->terminateThread();
    mSimulatorDecisionMakerThread->terminateThread();
    mSimulatorDateRangeDecisionMakerThread->terminateThread();

    simulatorPortfolioUpdateLastPricesTimer.stop();

    mSimulatorPortfolioLastPriceThread->wait();
    mSimulatorDecisionMakerThread->wait();
    mSimulatorDateRangeDecisionMakerThread->wait();
}

void MainWindow::startAutoPilot()
{
    const QString mode    = mAutoPilotSettingsEditor->value("Options/Mode", AUTO_PILOT_MODE_VIEW).toString();
    const QString account = mAutoPilotSettingsEditor->value("Options/Account", "").toString();

    mUserStorage->readLock();
    const Accounts& accounts    = mUserStorage->getAccounts();
    const Account   accountInfo = accounts.value(account);
    Account         anotherAccountInfo;

    mAutoPilotAccountId = accountInfo.id;

    if (mode == AUTO_PILOT_MODE_FOLLOW)
    {
        const QString anotherAccount = mAutoPilotSettingsEditor->value("Options/AnotherAccount", "").toString();
        anotherAccountInfo           = accounts.value(anotherAccount);

        mAutoPilotAnotherAccountId = anotherAccountInfo.id;
    }
    else
    {
        mAutoPilotAnotherAccountId = "-";
    }
    mUserStorage->readUnlock();

    if (mAutoPilotAccountId != "" && mAutoPilotAnotherAccountId != "")
    {
        ui->autoPilotActiveWidget->show();
        ui->autoPilotActiveSpinnerWidget->start();

        ui->startAutoPilotButton->setIcon(QIcon(":/assets/images/stop.png"));
        ui->startAutoPilotButton->setText(tr("Stop auto-pilot"));

        mOperationsThread->setAccountId(account, mAutoPilotAccountId);
        mLogsThread->setAccountId(account, mAutoPilotAccountId);
        mPortfolioThread->setAccountId(mAutoPilotAccountId);

        if (mode == AUTO_PILOT_MODE_INTERNAL)
        {
            mAutoPilotDecisionMakerThread->setAccountId(mAutoPilotAccountId);
            mHighLiquidityThread->setAccountId(mAutoPilotAccountId);
        }
        else if (mode == AUTO_PILOT_MODE_FOLLOW)
        {
            mFollowThread->setAccounts(mAutoPilotAccountId, mAutoPilotAnotherAccountId, anotherAccountInfo.name);
        }

        mAutoPilotDecisionMakerWidget->setAccountName(accountInfo.name);
        mAutoPilotDecisionMakerWidget->showSpinners();

        mOperationsThread->start();
        mLogsThread->start();
        mPortfolioThread->start();
        mAutoPilotPortfolioLastPriceThread->start();

        if (mode == AUTO_PILOT_MODE_INTERNAL)
        {
            mAutoPilotDecisionMakerThread->start();

            if (mConfig->isTradeHugeSpread())
            {
                mBiDirTradingControlThread->start();
            }

            if (mConfig->isTradeLiquidityEtfNightly())
            {
                mHighLiquidityThread->start();
            }
        }
        else if (mode == AUTO_PILOT_MODE_FOLLOW)
        {
            mFollowThread->start();
        }

        autoPilotPortfolioUpdateLastPricesTimer.start();

        mLogsThread->addLog(LOG_LEVEL_INFO, "", tr("Auto-pilot started"));
    }
}

void MainWindow::stopAutoPilot()
{
    mLogsThread->addLog(LOG_LEVEL_INFO, "", tr("Auto-pilot stopped"));

    ui->autoPilotActiveWidget->hide();
    ui->autoPilotActiveSpinnerWidget->stop();

    ui->startAutoPilotButton->setIcon(QIcon(":/assets/images/start.png"));
    ui->startAutoPilotButton->setText(tr("Start auto-pilot"));

    mOperationsThread->terminateThread();
    mLogsThread->terminateThread();
    mPortfolioThread->terminateThread();
    mAutoPilotPortfolioLastPriceThread->terminateThread();
    mAutoPilotDecisionMakerThread->terminateThread();
    mBiDirTradingControlThread->terminateThread();
    mHighLiquidityThread->terminateThread();
    mFollowThread->terminateThread();

    for (auto it = tradingThreads.constBegin(); it != tradingThreads.constEnd(); ++it)
    {
        it.value()->terminateThread();
    }

    for (auto it = biDirTradingThreads.constBegin(); it != biDirTradingThreads.constEnd(); ++it)
    {
        it.value()->terminateThread();
    }

    autoPilotPortfolioUpdateLastPricesTimer.stop();

    mOperationsThread->wait();
    mLogsThread->wait();
    mPortfolioThread->wait();
    mAutoPilotPortfolioLastPriceThread->wait();
    mAutoPilotDecisionMakerThread->wait();
    mBiDirTradingControlThread->wait();
    mHighLiquidityThread->wait();
    mFollowThread->wait();

    for (auto it = tradingThreads.constBegin(); it != tradingThreads.constEnd(); ++it)
    {
        it.value()->wait();
        delete it.value();
    }

    for (auto it = biDirTradingThreads.constBegin(); it != biDirTradingThreads.constEnd(); ++it)
    {
        it.value()->wait();
        delete it.value();
    }

    tradingThreads.clear();
    biDirTradingThreads.clear();
}

void MainWindow::simulatorStepProgressChanged(int current, int maximum) const
{
    ui->simulatorStepProgressBar->setMaximum(maximum);
    ui->simulatorStepProgressBar->setValue(current);
}

void MainWindow::simulatorTotalProgressChanged(int current, int maximum) const
{
    ui->simulatorTotalProgressBar->setMaximum(maximum);
    ui->simulatorTotalProgressBar->setValue(current);
}

void MainWindow::simulatorProgressChanged(int current, int maximum, const QString& remainingTime) const
{
    ui->simulatorRemainingTimeLabel->setText(remainingTime);
    ui->simulatorProgressBar->setMaximum(maximum);
    ui->simulatorProgressBar->setValue(current);
}

void MainWindow::simulatorResultFound(const QString& result, const QColor& color)
{
    qDebug() << "Intermediate simulation result:" << result;

    if (isActiveWindow() && ui->stackedWidget->currentWidget() == ui->simulationPage)
    {
        QRandomGenerator* generator = QRandomGenerator::global();

        // NOLINTBEGIN(readability-magic-numbers)
        const QPoint startPoint = ui->simulatorBestResultLabel->mapToGlobal(QPoint(0, 20));
        const QPoint endPoint   = startPoint + QPoint(generator->bounded(-200, 200), generator->bounded(250, 300));
        // NOLINTEND(readability-magic-numbers)

        FloatingLabel* label = new FloatingLabel(result, startPoint, endPoint, this);

        QPalette palette;
        palette.setColor(QPalette::WindowText, color);
        label->setPalette(palette);

        label->show();
    }
}

void MainWindow::simulatorBestResultChanged(const QString& bestResult, const QColor& color) const
{
    ui->simulatorBestResultLabel->setText(bestResult);

    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    ui->simulatorBestResultLabel->setPalette(palette);
}

void MainWindow::simulatorBestConfigFound()
{
    mSimulatorDecisionMakerWidget->bestConfigFound();
}

void MainWindow::simulatorOperationsRead(const QList<Operation>& operations)
{
    mSimulatorDecisionMakerWidget->operationsRead(operations);
}

void MainWindow::simulatorOperationsAdded(const QList<Operation>& operations)
{
    mSimulatorDecisionMakerWidget->operationsAdded(operations);
}

void MainWindow::simulatorLogsRead(const QList<LogEntry>& entries)
{
    mSimulatorDecisionMakerWidget->logsRead(entries);
}

void MainWindow::simulatorLogAdded(const LogEntry& entry)
{
    mSimulatorDecisionMakerWidget->logAdded(entry);
}

void MainWindow::simulatorPortfolioChanged(const Portfolio& portfolio)
{
    ui->simulatorWaitingStackedWidget->setCurrentWidget(ui->simulatorWorkingPage);

    mSimulatorDecisionMakerWidget->portfolioChanged(portfolio);
    mSimulatorPortfolioLastPriceThread->portfolioChanged(portfolio);
}

void MainWindow::simulatorPortfolioLastPriceChanged(const QString& instrumentId, float price)
{
    mSimulatorDecisionMakerWidget->lastPriceChanged(instrumentId, price);
}

void MainWindow::autoPilotOperationsRead(const QList<Operation>& operations)
{
    mAutoPilotDecisionMakerWidget->operationsRead(operations);
}

void MainWindow::autoPilotOperationsAdded(const QList<Operation>& operations)
{
    mAutoPilotDecisionMakerWidget->operationsAdded(operations);
}

void MainWindow::autoPilotLogsRead(const QList<LogEntry>& entries)
{
    mAutoPilotDecisionMakerWidget->logsRead(entries);
}

void MainWindow::autoPilotLogAdded(const LogEntry& entry)
{
    mAutoPilotDecisionMakerWidget->logAdded(entry);
}

void MainWindow::autoPilotPortfolioChanged(const Portfolio& portfolio)
{
    mAutoPilotDecisionMakerWidget->portfolioChanged(portfolio);
    mAutoPilotPortfolioLastPriceThread->portfolioChanged(portfolio);
}

void MainWindow::autoPilotPortfolioLastPriceChanged(const QString& instrumentId, float price)
{
    mAutoPilotDecisionMakerWidget->lastPriceChanged(instrumentId, price);
}

void MainWindow::autoPilotTradeInstruments(const InstrumentsForTrading& instruments)
{
    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        if (!biDirTradingThreads.contains(instrumentId))
        {
            const TradingInfo& tradingInfo = it.value();

            if (tradingInfo.expectedCost == 0)
            {
                mAutoPilotDecisionMakerThread->notifyAboutSell(instrumentId);
            }

            ITradingThread* tradingThread = tradingThreads.value(instrumentId);

            if (tradingThread == nullptr)
            {
                tradingThread = mTradingThreadFactory->newInstance(
                    mInstrumentsStorage,
                    mUserStorage,
                    mTimeUtils,
                    mGrpcClient,
                    mGrpcRetryClient,
                    mLogsThread,
                    mAutoPilotAccountId,
                    instrumentId,
                    tradingInfo.asapMode,
                    tradingInfo.avgPrice,
                    tradingInfo.price,
                    tradingInfo.expectedCost,
                    tradingInfo.cause,
                    this
                );

                connect(
                    tradingThread, SIGNAL(tradingCompleted(const QString&)), this, SLOT(autoPilotTradingCompleted(const QString&))
                );

                tradingThreads[instrumentId] = tradingThread;
                tradingThread->start();
            }
            else
            {
                tradingThread->setAsapMode(tradingInfo.asapMode);
                tradingThread->setAvgPrice(tradingInfo.avgPrice);
                tradingThread->setExpectedCost(tradingInfo.expectedCost, tradingInfo.cause);
            }
        }
    }
}

void MainWindow::autoPilotTradingCompleted(const QString& instrumentId)
{
    ITradingThread* tradingThread = tradingThreads.take(instrumentId);

    if (tradingThread != nullptr)
    {
        tradingThread->wait();
        delete tradingThread;
    }
}

void MainWindow::autoPilotBiDirTradeInstruments(const InstrumentsForBiDirTrading& instruments)
{
    QStringList tradingThreadsToKill;

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        ITradingThread* tradingThread = tradingThreads.value(instrumentId);

        if (tradingThread != nullptr)
        {
            tradingThread->terminateThread();

            tradingThreadsToKill.append(instrumentId);
        }
    }

    for (const QString& instrumentId : tradingThreadsToKill)
    {
        ITradingThread* tradingThread = tradingThreads.take(instrumentId);
        Q_ASSERT_X(tradingThread != nullptr, __FUNCTION__, "Unexpected behavior");

        tradingThread->wait();
        delete tradingThread;
    }

    for (auto it = biDirTradingThreads.constBegin(); it != biDirTradingThreads.constEnd(); ++it)
    {
        const QString& instrumentId = it.key();

        if (!instruments.contains(instrumentId))
        {
            it.value()->terminateTrading();
        }
    }

    for (auto it = instruments.constBegin(); it != instruments.constEnd(); ++it)
    {
        const QString&          instrumentId     = it.key();
        const BiDirTradingInfo& biDirTradingInfo = it.value();

        IBiDirTradingThread* biDirTradingThread = biDirTradingThreads.value(instrumentId);

        if (biDirTradingThread == nullptr)
        {
            biDirTradingThread = mBiDirTradingThreadFactory->newInstance(
                mInstrumentsStorage,
                mUserStorage,
                mConfig,
                mTimeUtils,
                mTradeUtils,
                mGrpcClient,
                mGrpcRetryClient,
                mLogsThread,
                mAutoPilotAccountId,
                biDirTradingInfo.stock,
                biDirTradingInfo.cause,
                this
            );

            connect(
                biDirTradingThread,
                SIGNAL(tradingCompleted(const QString&)),
                this,
                SLOT(autoPilotBiDirTradingCompleted(const QString&))
            );

            biDirTradingThreads[instrumentId] = biDirTradingThread;
            biDirTradingThread->start();
        }
    }
}

void MainWindow::autoPilotBiDirTradingCompleted(const QString& instrumentId)
{
    IBiDirTradingThread* biDirTradingThread = biDirTradingThreads.take(instrumentId);

    if (biDirTradingThread != nullptr)
    {
        biDirTradingThread->wait();
        delete biDirTradingThread;
    }
}

void MainWindow::on_actionAuth_triggered()
{
    ui->actionAuth->setEnabled(false);

    userUpdateTimerTicked();
    priceCollectTimerTicked();

    mLastPriceThread->start();

    userUpdateTimer.start();
    priceCollectTimer.start();
    makeDecisionTimer.start();
    stocksTableUpdateAllTimer.start();
    stocksTableUpdateLastPricesTimer.start();

    if (mSimulatorSettingsEditor->value("General/Enabled", false).toBool())
    {
        startSimulator();
    }

    if (mAutoPilotSettingsEditor->value("General/Enabled", false).toBool())
    {
        startAutoPilot();
    }
}

void MainWindow::on_actionStocksPage_toggled(bool checked)
{
    if (checked)
    {
        ui->stackedWidget->setCurrentWidget(ui->stocksPage);
    }

    updateStackWidgetToolbar();
}

void MainWindow::on_actionSimulationPage_toggled(bool checked)
{
    if (checked)
    {
        ui->stackedWidget->setCurrentWidget(ui->simulationPage);
    }

    updateStackWidgetToolbar();
}

void MainWindow::on_actionAutoPilotPage_toggled(bool checked)
{
    if (checked)
    {
        ui->stackedWidget->setCurrentWidget(ui->autoPilotPage);
    }

    updateStackWidgetToolbar();
}

void MainWindow::on_actionSettings_triggered()
{
    mConfigForSettingsDialog->assign(mConfig);

    const std::shared_ptr<ISettingsDialog> dialog = mSettingsDialogFactory->newInstance(
        SettingsDialogFactoryNewInstanceArgsMore15(
            mConfigForSettingsDialog,
            mDecisionMakerConfigWidgetFactory,
            mBuyDecision1ConfigWidgetFactory,
            mBuyDecision2ConfigWidgetFactory,
            mBuyDecision3ConfigWidgetFactory,
            mBuyDecision4ConfigWidgetFactory,
            mBuyDecision5ConfigWidgetFactory,
            mBuyDecision6ConfigWidgetFactory,
            mBuyDecision7ConfigWidgetFactory,
            mBuyDecision8ConfigWidgetFactory,
            mSellDecision1ConfigWidgetFactory,
            mSellDecision2ConfigWidgetFactory,
            mSellDecision3ConfigWidgetFactory,
            mSellDecision4ConfigWidgetFactory,
            mSellDecision5ConfigWidgetFactory,
            mUserStorage
        ),
        this
    );

    dialog->updateUiFromConfig();

    if (dialog->exec() == QDialog::Accepted)
    {
        qInfo() << "Settings applied";

        mConfig->assign(mConfigForSettingsDialog);
        mConfig->save(mSettingsEditor);

        applyConfig();
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
    if (!ui->simulationActiveSpinnerWidget->isSpinning())
    {
        const std::shared_ptr<IStartSimulationDialog> dialog = mStartSimulationDialogFactory->newInstance(mSettingsEditor, this);

        if (dialog->exec() == QDialog::Accepted)
        {
            // clang-format off
            mSimulatorSettingsEditor->setValue("General/Enabled",    true);
            mSimulatorSettingsEditor->setValue("Options/StartMoney", dialog->startMoney());
            mSimulatorSettingsEditor->setValue("Options/Mode",       dialog->mode());
            mSimulatorSettingsEditor->setValue("Options/FromDate",   dialog->fromDate().toString(DATE_FORMAT));
            mSimulatorSettingsEditor->setValue("Options/ToDate",     dialog->toDate().toString(DATE_FORMAT));
            mSimulatorSettingsEditor->setValue("Options/BestConfig", dialog->bestConfig());
            // clang-format on

            mSimulatorDecisionMakerThread->reset();
            mSimulatorDateRangeDecisionMakerThread->reset();

            startSimulator();
        }
    }
    else
    {
        if (mMessageBoxUtils->question(this, tr("Stop simulation"), tr("Do you really want to stop simulation?")) ==
            QMessageBox::Yes)
        {
            mSimulatorSettingsEditor->setValue("General/Enabled", false);

            stopSimulator();
        }
    }
}

void MainWindow::on_startAutoPilotButton_clicked()
{
    if (!ui->autoPilotActiveSpinnerWidget->isSpinning())
    {
        const std::shared_ptr<IStartAutoPilotDialog> dialog =
            mStartAutoPilotDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, mSettingsEditor, this);

        if (dialog->exec() == QDialog::Accepted)
        {
            // clang-format off
            mAutoPilotSettingsEditor->setValue("General/Enabled",        true);
            mAutoPilotSettingsEditor->setValue("Options/Mode",           dialog->mode());
            mAutoPilotSettingsEditor->setValue("Options/Account",        dialog->account());
            mAutoPilotSettingsEditor->setValue("Options/AnotherAccount", dialog->anotherAccount());
            // clang-format on

            startAutoPilot();
        }
    }
    else
    {
        if (mMessageBoxUtils->question(this, tr("Stop auto-pilot"), tr("Do you really want to stop auto-pilot?")) ==
            QMessageBox::Yes)
        {
            mAutoPilotSettingsEditor->setValue("General/Enabled", false);

            stopAutoPilot();
        }
    }
}

enum DatabaseType : qint8
{
    DATABASE_TYPE_USER,
    DATABASE_TYPE_STOCKS,
    DATABASE_TYPE_INSTRUMENT,
    DATABASE_TYPE_LOGOS
};

struct ReadDatabasesInfo
{
    explicit ReadDatabasesInfo(
        IUserStorage*        _userStorage,
        IStocksStorage*      _stocksStorage,
        IInstrumentsStorage* _instrumentsStoragen,
        ILogosStorage*       _logosStorage
    ) :
        userStorage(_userStorage),
        stocksStorage(_stocksStorage),
        instrumentsStorage(_instrumentsStoragen),
        logosStorage(_logosStorage)
    {
    }

    IUserStorage*        userStorage;
    IStocksStorage*      stocksStorage;
    IInstrumentsStorage* instrumentsStorage;
    ILogosStorage*       logosStorage;
};

static void readDatabasesForParallel(
    QThread* parentThread,
    int /*threadId*/,
    DatabaseType* databases, // NOLINT(readability-non-const-parameter)
    int /*size*/,
    int   start,
    int   end,
    void* additionalArgs
)
{
    ReadDatabasesInfo* readDatabasesInfo = reinterpret_cast<ReadDatabasesInfo*>(additionalArgs);

    for (int i = start; i < end && !parentThread->isInterruptionRequested(); ++i)
    {
        const DatabaseType dbType = databases[i];

        if (dbType == DATABASE_TYPE_USER)
        {
            readDatabasesInfo->userStorage->writeLock();
            readDatabasesInfo->userStorage->readFromDatabase();
            readDatabasesInfo->userStorage->writeUnlock();
        }
        else if (dbType == DATABASE_TYPE_STOCKS)
        {
            readDatabasesInfo->stocksStorage->writeLock();
            readDatabasesInfo->stocksStorage->readFromDatabase();
            readDatabasesInfo->stocksStorage->writeUnlock();
        }
        else if (dbType == DATABASE_TYPE_INSTRUMENT)
        {
            readDatabasesInfo->instrumentsStorage->writeLock();
            readDatabasesInfo->instrumentsStorage->readFromDatabase();
            readDatabasesInfo->instrumentsStorage->writeUnlock();
        }
        else if (dbType == DATABASE_TYPE_LOGOS)
        {
            readDatabasesInfo->logosStorage->writeLock();
            readDatabasesInfo->logosStorage->readFromDatabase();
            readDatabasesInfo->logosStorage->writeUnlock();
        }
    }
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    QList<DatabaseType> databases;
    databases << DATABASE_TYPE_USER << DATABASE_TYPE_STOCKS << DATABASE_TYPE_INSTRUMENT << DATABASE_TYPE_LOGOS;

    ReadDatabasesInfo readDatabasesInfo(mUserStorage, mStocksStorage, mInstrumentsStorage, mLogosStorage);
    processInParallel(QThread::currentThread(), databases, readDatabasesForParallel, &readDatabasesInfo);

    mStocksStorage->readLock();
    mStocksStorage->assignLogos();
    mStocksStorage->readUnlock();

    updateStocksTableWidget();

    cleanupTimer.start(CLEANUP_INTERVAL);
    cleanupTimerTicked();

    userUpdateTimer.setInterval(USER_UPDATE_INTERVAL);
    priceCollectTimer.setInterval(PRICE_COLLECT_INTERVAL);
    stocksTableUpdateAllTimer.setInterval(STOCKS_TABLE_UPDATE_ALL_INTERVAL);
    stocksTableUpdateLastPricesTimer.setInterval(STOCKS_TABLE_UPDATE_LAST_PRICES_INTERVAL);
    simulatorPortfolioUpdateLastPricesTimer.setInterval(PORTFOLIO_UPDATE_LAST_PRICES_INTERVAL);
    autoPilotPortfolioUpdateLastPricesTimer.setInterval(PORTFOLIO_UPDATE_LAST_PRICES_INTERVAL);

    on_actionAuth_triggered();
}

void MainWindow::updateStocksTableWidget()
{
    mStocksStorage->readLock();
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    if (!stocks.isEmpty())
    {
        const QDateTime dateChangeTime = mStocksControlsWidget->getDateChangeTime();

        mStocksStorage->obtainStocksDatePrice(dateChangeTime.toMSecsSinceEpoch());
        mStocksTableWidget->setDateChangeTooltip(tr("From: %1").arg(dateChangeTime.toString(DATETIME_FORMAT)));
        mStocksTableWidget->updateTable(stocks);
        mStocksStorage->readUnlock();

        ui->waitingSpinnerWidget->stop();
        ui->waitingStackedWidget->setCurrentWidget(ui->workingPage);
    }
    else
    {
        mStocksStorage->readUnlock();

        ui->waitingSpinnerWidget->start();
        ui->waitingStackedWidget->setCurrentWidget(ui->waitingPage);
    }
}

void MainWindow::updateStackWidgetToolbar() const
{
    ui->actionStocksPage->setChecked(ui->stackedWidget->currentWidget() == ui->stocksPage);
    ui->actionStocksPage->setIcon(QIcon(
        ui->stackedWidget->currentWidget() == ui->stocksPage ? ":/assets/images/stocks_selected.png"
                                                             : ":/assets/images/stocks.png"
    ));

    ui->actionSimulationPage->setChecked(ui->stackedWidget->currentWidget() == ui->simulationPage);
    ui->actionSimulationPage->setIcon(QIcon(
        ui->stackedWidget->currentWidget() == ui->simulationPage ? ":/assets/images/simulation_selected.png"
                                                                 : ":/assets/images/simulation.png"
    ));

    ui->actionAutoPilotPage->setChecked(ui->stackedWidget->currentWidget() == ui->autoPilotPage);
    ui->actionAutoPilotPage->setIcon(QIcon(
        ui->stackedWidget->currentWidget() == ui->autoPilotPage ? ":/assets/images/auto_pilot_selected.png"
                                                                : ":/assets/images/auto_pilot.png"
    ));
}

void MainWindow::applyConfig()
{
    mAutorunEnabler->setEnabled(mConfig->isAutorun());
    setCpuCount(mConfig->getCpuUsage());
    makeDecisionTimer.setInterval(mConfig->getMakeDecisionTimeout() * ONE_MINUTE);

    if (!mConfig->isTradeHugeSpread())
    {
        mBiDirTradingControlThread->terminateThread();
        mBiDirTradingControlThread->wait();

        autoPilotBiDirTradeInstruments(InstrumentsForBiDirTrading());
    }

    mSimulatorDecisionMakerWidget->refreshOperationsBackground();
    mAutoPilotDecisionMakerWidget->refreshOperationsBackground();
}

void MainWindow::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("MainWindow/geometry",    saveGeometry());
    mSettingsEditor->setValue("MainWindow/windowState", saveState());
    mSettingsEditor->setValue("MainWindow/pageIndex",   ui->stackedWidget->currentIndex());
    // clang-format on

    mStocksControlsWidget->saveWindowState("MainWindow/StocksControlsWidget");
    mStocksTableWidget->saveWindowState("MainWindow/StocksTableWidget");
    mSimulatorDecisionMakerWidget->saveWindowState("MainWindow/Simulator");
    mAutoPilotDecisionMakerWidget->saveWindowState("MainWindow/AutoPilot");
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    restoreGeometry(mSettingsEditor->value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(mSettingsEditor->value("MainWindow/windowState", QByteArray()).toByteArray());
    ui->stackedWidget->setCurrentIndex(mSettingsEditor->value("MainWindow/pageIndex", 0).toInt());

    mStocksControlsWidget->loadWindowState("MainWindow/StocksControlsWidget");
    mStocksTableWidget->loadWindowState("MainWindow/StocksTableWidget");
    mSimulatorDecisionMakerWidget->loadWindowState("MainWindow/Simulator");
    mAutoPilotDecisionMakerWidget->loadWindowState("MainWindow/AutoPilot");

    updateStackWidgetToolbar();
}
