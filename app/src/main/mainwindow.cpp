#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>



const QColor      GREY_COLOR                               = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static
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

#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



MainWindow::MainWindow(
    IConfig*                           config,
    IConfig*                           configForSettingsDialog,
    IConfig*                           configForSimulation,
    IAuthDialogFactory*                authDialogFactory,
    ISettingsDialogFactory*            settingsDialogFactory,
    IOrderWavesDialogFactory*          orderWavesDialogFactory,
    IStartSimulationDialogFactory*     startSimulationDialogFactory,
    IStartAutoPilotDialogFactory*      startAutoPilotDialogFactory,
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    IStockTableItemWidgetFactory*      stockTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    IStocksTableRecordFactory*         stockTableRecordFactory,
    IStocksControlsWidgetFactory*      stocksControlsWidgetFactory,
    IStocksTableWidgetFactory*         stocksTableWidgetFactory,
    IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
    IAccountChartWidgetFactory*        accountChartWidgetFactory,
    ILogsTableWidgetFactory*           logsTableWidgetFactory,
    IPortfolioTableWidgetFactory*      portfolioTableWidgetFactory,
    IDecisionMakerWidgetFactory*       decisionMakerWidgetFactory,
    ITrayIconFactory*                  trayIconFactory,
    IUserStorage*                      userStorage,
    IStocksStorage*                    stocksStorage,
    IHttpClient*                       httpClient,
    IGrpcClient*                       grpcClient,
    IUserUpdateThread*                 userUpdateThread,
    IPriceCollectThread*               priceCollectThread,
    ILastPriceThread*                  lastPriceThread,
    ICleanupThread*                    cleanupThread,
    IMakeDecisionThread*               makeDecisionThread,
    IOrderBookThread*                  orderBookThread,
    IMessageBoxUtils*                  messageBoxUtils,
    ISettingsEditor*                   settingsEditor,
    IAutorunEnabler*                   autorunEnabler,
    QWidget*                           parent
) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    authFailedDialogShown(false),
    userUpdateTimer(new QTimer(this)),
    priceCollectTimer(new QTimer(this)),
    cleanupTimer(new QTimer(this)),
    makeDecisionTimer(new QTimer(this)),
    stocksTableUpdateAllTimer(new QTimer(this)),
    stocksTableUpdateLastPricesTimer(new QTimer(this)),
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
    mSellDecision1ConfigWidgetFactory(sellDecision1ConfigWidgetFactory),
    mSellDecision2ConfigWidgetFactory(sellDecision2ConfigWidgetFactory),
    mSellDecision3ConfigWidgetFactory(sellDecision3ConfigWidgetFactory),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mUserUpdateThread(userUpdateThread),
    mPriceCollectThread(priceCollectThread),
    mLastPriceThread(lastPriceThread),
    mCleanupThread(cleanupThread),
    mMakeDecisionThread(makeDecisionThread),
    mOrderBookThread(orderBookThread),
    mMessageBoxUtils(messageBoxUtils),
    mSettingsEditor(settingsEditor),
    mAutorunEnabler(autorunEnabler)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

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

    mStocksControlsWidget = stocksControlsWidgetFactory->newInstance(mStocksStorage, mSettingsEditor, this);
    mStocksTableWidget    = stocksTableWidgetFactory->newInstance(
        stockTableRecordFactory,
        stockTableItemWidgetFactory,
        actionsTableItemWidgetFactory,
        orderWavesDialogFactory,
        orderWavesWidgetFactory,
        mUserStorage,
        mOrderBookThread,
        mHttpClient,
        mSettingsEditor,
        this
    );
    mSimulatorDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsTableWidgetFactory,
        portfolioTableWidgetFactory,
        mSettingsEditor,
        this
    );
    mAutoPilotDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsTableWidgetFactory,
        portfolioTableWidgetFactory,
        mSettingsEditor,
        this
    );

    ui->layoutForStocksControlsWidget->addWidget(mStocksControlsWidget);
    ui->layoutForStocksTableWidget->addWidget(mStocksTableWidget);
    ui->layoutForSimulatorDecisionMaker->addWidget(mSimulatorDecisionMakerWidget);
    ui->layoutForAutoPilotDecisionMaker->addWidget(mAutoPilotDecisionMakerWidget);

    mTrayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(mTrayIcon,                        SIGNAL(activated(QSystemTrayIcon::ActivationReason)),                                         this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon,                        SIGNAL(trayIconShowClicked()),                                                                this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon,                        SIGNAL(trayIconExitClicked()),                                                                this, SLOT(trayIconExitClicked()));
    connect(mGrpcClient,                      SIGNAL(authFailed(grpc::StatusCode, const QString&, const std::string&, const std::string&)), this, SLOT(authFailed(grpc::StatusCode, const QString&, const std::string&, const std::string&)));
    connect(userUpdateTimer,                  SIGNAL(timeout()),                                                                            this, SLOT(userUpdateTimerTicked()));
    connect(priceCollectTimer,                SIGNAL(timeout()),                                                                            this, SLOT(priceCollectTimerTicked()));
    connect(cleanupTimer,                     SIGNAL(timeout()),                                                                            this, SLOT(cleanupTimerTicked()));
    connect(makeDecisionTimer,                SIGNAL(timeout()),                                                                            this, SLOT(makeDecisionTimerTicked()));
    connect(stocksTableUpdateAllTimer,        SIGNAL(timeout()),                                                                            this, SLOT(stocksTableUpdateAllTimerTicked()));
    connect(stocksTableUpdateLastPricesTimer, SIGNAL(timeout()),                                                                            this, SLOT(stocksTableUpdateLastPricesTimerTicked()));
    connect(mPriceCollectThread,              SIGNAL(notifyStocksProgress(const QString&)),                                                 this, SLOT(notifyStocksProgress(const QString&)));
    connect(mPriceCollectThread,              SIGNAL(stocksChanged()),                                                                      this, SLOT(stocksChanged()));
    connect(mPriceCollectThread,              SIGNAL(pricesChanged()),                                                                      this, SLOT(pricesChanged()));
    connect(mPriceCollectThread,              SIGNAL(periodicDataChanged()),                                                                this, SLOT(periodicDataChanged()));
    connect(mLastPriceThread,                 SIGNAL(lastPriceChanged(const QString&)),                                                     this, SLOT(lastPriceChanged(const QString&)));
    connect(mStocksControlsWidget,            SIGNAL(dateChangeDateTimeChanged(const QDateTime&)),                                          this, SLOT(dateChangeDateTimeChanged(const QDateTime&)));
    connect(mStocksControlsWidget,            SIGNAL(filterChanged(const Filter&)),                                                         this, SLOT(filterChanged(const Filter&)));
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

    mUserUpdateThread->requestInterruption();
    mPriceCollectThread->requestInterruption();
    mLastPriceThread->terminateThread();
    mCleanupThread->requestInterruption();
    mMakeDecisionThread->requestInterruption();

    mUserUpdateThread->wait();
    mPriceCollectThread->wait();
    mLastPriceThread->wait();
    mCleanupThread->wait();
    mMakeDecisionThread->wait();

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

    Q_UNUSED(title);
    Q_UNUSED(msg);

#ifdef Q_OS_WINDOWS
    mTrayIcon->showMessage(title, msg);
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

void MainWindow::authFailed(
    grpc::StatusCode errorCode, const QString& errorCodeString, const std::string& errorMessage, const std::string& errorDetails
)
{
    qWarning() << "Authorization failed with code:" << errorCode << errorCodeString
               << "message:" << QString::fromStdString(errorMessage) << "details:" << QString::fromStdString(errorDetails);

    if (authFailedDialogShown)
    {
        return;
    }

    userUpdateTimer->stop();
    mUserUpdateThread->requestInterruption();
    mUserUpdateThread->wait();

    priceCollectTimer->stop();
    mPriceCollectThread->requestInterruption();
    mPriceCollectThread->wait();

    mLastPriceThread->terminateThread();
    mLastPriceThread->wait();

    makeDecisionTimer->stop();
    mMakeDecisionThread->requestInterruption();
    mMakeDecisionThread->wait();

    stocksTableUpdateAllTimer->stop();
    stocksTableUpdateLastPricesTimer->stop();

    ui->actionAuth->setEnabled(true);
    ui->waitingSpinnerWidget->setText(tr("Waiting for authorization"));
    trayIconShowClicked();

    authFailedDialogShown = true;

    const std::shared_ptr<IAuthDialog> dialog = mAuthDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, this);

    if (dialog->exec() == QDialog::Accepted)
    {
        mUserStorage->setToken(dialog->getToken());

        on_actionAuth_triggered();
    }

    authFailedDialogShown = false;
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

void MainWindow::cleanupTimerTicked()
{
    qInfo() << "Cleanup timer ticked";

    mCleanupThread->start();
}

void MainWindow::makeDecisionTimerTicked()
{
    qDebug() << "Make decision timer ticked";

    mMakeDecisionThread->start();
}

void MainWindow::stocksTableUpdateAllTimerTicked()
{
    qDebug() << "Stocks table update all timer ticked";

    mStocksTableWidget->updateAll(mStocksControlsWidget->getFilter());
}

void MainWindow::stocksTableUpdateLastPricesTimerTicked()
{
    qDebug() << "Stocks table update timer ticked";

    mStocksTableWidget->updateLastPrices(mStocksControlsWidget->getFilter());
}

void MainWindow::notifyStocksProgress(const QString& message) const
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
    mStocksTableWidget->updatePrices(mStocksControlsWidget->getFilter());
}

void MainWindow::periodicDataChanged()
{
    mStocksTableWidget->updatePeriodicData(mStocksControlsWidget->getFilter());
}

void MainWindow::lastPriceChanged(const QString& uid)
{
    mStocksTableWidget->lastPriceChanged(uid);
}

void MainWindow::dateChangeDateTimeChanged(const QDateTime& dateTime)
{
    mStocksTableWidget->setDateChangeTooltip(tr("From: %1").arg(dateTime.toString(DATETIME_FORMAT)));
    mStocksTableWidget->updatePrices(mStocksControlsWidget->getFilter());
}

void MainWindow::filterChanged(const Filter& filter)
{
    mStocksTableWidget->filterChanged(filter);
}

void MainWindow::on_actionAuth_triggered()
{
    ui->actionAuth->setEnabled(false);

    userUpdateTimer->start();
    userUpdateTimerTicked();

    priceCollectTimer->start();
    priceCollectTimerTicked();

    mLastPriceThread->start();

    makeDecisionTimer->start();
    makeDecisionTimerTicked();

    stocksTableUpdateAllTimer->start();
    stocksTableUpdateLastPricesTimer->start();
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
        mConfigForSettingsDialog,
        mDecisionMakerConfigWidgetFactory,
        mBuyDecision1ConfigWidgetFactory,
        mBuyDecision2ConfigWidgetFactory,
        mBuyDecision3ConfigWidgetFactory,
        mSellDecision1ConfigWidgetFactory,
        mSellDecision2ConfigWidgetFactory,
        mSellDecision3ConfigWidgetFactory,
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
            ui->simulationActiveWidget->show();
            ui->simulationActiveSpinnerWidget->start();

            ui->startSimulationButton->setIcon(QIcon(":/assets/images/stop.png"));
            ui->startSimulationButton->setText(tr("Stop simulation"));

            // TODO: Start simulation
        }
    }
    else
    {
        if (mMessageBoxUtils->question(this, tr("Stop simulation"), tr("Do you really want to stop simulation?")) ==
            QMessageBox::Yes)
        {
            ui->simulationActiveWidget->hide();
            ui->simulationActiveSpinnerWidget->stop();

            ui->startSimulationButton->setIcon(QIcon(":/assets/images/start.png"));
            ui->startSimulationButton->setText(tr("Start simulation"));

            // TODO: Stop simulation
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
            ui->autoPilotActiveWidget->show();
            ui->autoPilotActiveSpinnerWidget->start();

            ui->startAutoPilotButton->setIcon(QIcon(":/assets/images/stop.png"));
            ui->startAutoPilotButton->setText(tr("Stop auto-pilot"));

            // TODO: Start auto-pilot
        }
    }
    else
    {
        if (mMessageBoxUtils->question(this, tr("Stop auto-pilot"), tr("Do you really want to stop auto-pilot?")) ==
            QMessageBox::Yes)
        {
            ui->autoPilotActiveWidget->hide();
            ui->autoPilotActiveSpinnerWidget->stop();

            ui->startAutoPilotButton->setIcon(QIcon(":/assets/images/start.png"));
            ui->startAutoPilotButton->setText(tr("Start auto-pilot"));

            // TODO: Stop auto-pilot
        }
    }
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mUserStorage->readFromDatabase();
    mStocksStorage->readFromDatabase();

    updateStocksTableWidget();

    userUpdateTimer->setInterval(USER_UPDATE_INTERVAL);
    priceCollectTimer->setInterval(PRICE_COLLECT_INTERVAL);

    cleanupTimer->start(CLEANUP_INTERVAL);
    cleanupTimerTicked();

    stocksTableUpdateAllTimer->setInterval(STOCKS_TABLE_UPDATE_ALL_INTERVAL);
    stocksTableUpdateLastPricesTimer->setInterval(STOCKS_TABLE_UPDATE_LAST_PRICES_INTERVAL);

    on_actionAuth_triggered();
}

void MainWindow::updateStocksTableWidget()
{
    const QMutexLocker   lock(mStocksStorage->getMutex());
    const QList<Stock*>& stocks = mStocksStorage->getStocks();

    if (!stocks.isEmpty())
    {
        const QDateTime dateChangeTime = mStocksControlsWidget->getDateChangeTime();

        mStocksStorage->obtainStocksDatePrice(dateChangeTime.toMSecsSinceEpoch());
        mStocksTableWidget->setDateChangeTooltip(tr("From: %1").arg(dateChangeTime.toString(DATETIME_FORMAT)));
        mStocksTableWidget->updateTable(stocks, mStocksControlsWidget->getFilter());

        ui->stackedWidget->show();
        ui->waitingSpinnerWidget->hide();
        ui->waitingSpinnerWidget->stop();
    }
    else
    {
        ui->stackedWidget->hide();
        ui->waitingSpinnerWidget->show();
        ui->waitingSpinnerWidget->start();
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
    makeDecisionTimer->setInterval(mConfig->getMakeDecisionTimeout() * ONE_MINUTE);
    mAutorunEnabler->setEnabled(mConfig->isAutorun());
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
