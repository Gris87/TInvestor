#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>



const QColor      GREY_COLOR                               = QColor("#AFC2D7"); // clazy:exclude=non-pod-global-static
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
constexpr qint64  KEEP_MONEY_CHANGE_DELAY                  = MS_IN_SECOND;      // 1 second

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
    IInstrumentWidgetFactory*          instrumentWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
    IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
    IStocksTableRecordFactory*         stockTableRecordFactory,
    IStocksControlsWidgetFactory*      stocksControlsWidgetFactory,
    IStocksTableWidgetFactory*         stocksTableWidgetFactory,
    IOperationsTableWidgetFactory*     operationsTableWidgetFactory,
    IAccountChartWidgetFactory*        accountChartWidgetFactory,
    ILogsTableWidgetFactory*           logsTableWidgetFactory,
    IPortfolioTreeWidgetFactory*       portfolioTreeWidgetFactory,
    IOperationsTableRecordFactory*     operationsTableRecordFactory,
    IPortfolioTreeRecordFactory*       portfolioTreeRecordFactory,
    IDecisionMakerWidgetFactory*       decisionMakerWidgetFactory,
    ITrayIconFactory*                  trayIconFactory,
    IUserStorage*                      userStorage,
    IStocksStorage*                    stocksStorage,
    IInstrumentsStorage*               instrumentsStorage,
    IHttpClient*                       httpClient,
    IGrpcClient*                       grpcClient,
    ICleanupThread*                    cleanupThread,
    IUserUpdateThread*                 userUpdateThread,
    IPriceCollectThread*               priceCollectThread,
    ILastPriceThread*                  lastPriceThread,
    IOperationsThread*                 operationsThread,
    IPortfolioThread*                  portfolioThread,
    IPortfolioLastPriceThread*         portfolioLastPriceThread,
    IMakeDecisionThread*               makeDecisionThread,
    IOrderBookThread*                  orderBookThread,
    IFileDialogFactory*                fileDialogFactory,
    IMessageBoxUtils*                  messageBoxUtils,
    ISettingsEditor*                   settingsEditor,
    ISettingsEditor*                   simulatorSettingsEditor,
    ISettingsEditor*                   autoPilotSettingsEditor,
    IAutorunEnabler*                   autorunEnabler,
    QWidget*                           parent
) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    authFailedDialogShown(false),
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
    mInstrumentsStorage(instrumentsStorage),
    mHttpClient(httpClient),
    mGrpcClient(grpcClient),
    mCleanupThread(cleanupThread),
    mUserUpdateThread(userUpdateThread),
    mPriceCollectThread(priceCollectThread),
    mLastPriceThread(lastPriceThread),
    mOperationsThread(operationsThread),
    mPortfolioThread(portfolioThread),
    mPortfolioLastPriceThread(portfolioLastPriceThread),
    mMakeDecisionThread(makeDecisionThread),
    mOrderBookThread(orderBookThread),
    mFileDialogFactory(fileDialogFactory),
    mMessageBoxUtils(messageBoxUtils),
    mSettingsEditor(settingsEditor),
    mSimulatorSettingsEditor(simulatorSettingsEditor),
    mAutoPilotSettingsEditor(autoPilotSettingsEditor),
    mAutorunEnabler(autorunEnabler)
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

    mStocksControlsWidget = stocksControlsWidgetFactory->newInstance(mStocksStorage, mSettingsEditor, this);
    mStocksTableWidget    = stocksTableWidgetFactory->newInstance(
        stockTableRecordFactory,
        instrumentTableItemWidgetFactory,
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
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsTableWidgetFactory,
        portfolioTreeWidgetFactory,
        operationsTableRecordFactory,
        portfolioTreeRecordFactory,
        instrumentWidgetFactory,
        instrumentTableItemWidgetFactory,
        mUserStorage,
        mInstrumentsStorage,
        mFileDialogFactory,
        mSettingsEditor,
        this
    );
    mAutoPilotDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(
        operationsTableWidgetFactory,
        accountChartWidgetFactory,
        logsTableWidgetFactory,
        portfolioTreeWidgetFactory,
        operationsTableRecordFactory,
        portfolioTreeRecordFactory,
        instrumentWidgetFactory,
        instrumentTableItemWidgetFactory,
        mUserStorage,
        mInstrumentsStorage,
        mFileDialogFactory,
        mSettingsEditor,
        this
    );

    ui->layoutForStocksControlsWidget->addWidget(mStocksControlsWidget);
    ui->layoutForStocksTableWidget->addWidget(mStocksTableWidget);
    ui->layoutForSimulatorDecisionMaker->addWidget(mSimulatorDecisionMakerWidget);
    ui->layoutForAutoPilotDecisionMaker->addWidget(mAutoPilotDecisionMakerWidget);

    mTrayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(mTrayIcon,                         SIGNAL(activated(QSystemTrayIcon::ActivationReason)),                                         this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon,                         SIGNAL(trayIconShowClicked()),                                                                this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon,                         SIGNAL(trayIconExitClicked()),                                                                this, SLOT(trayIconExitClicked()));
    connect(mGrpcClient,                       SIGNAL(authFailed(grpc::StatusCode, const QString&, const std::string&, const std::string&)), this, SLOT(authFailed(grpc::StatusCode, const QString&, const std::string&, const std::string&)));
    connect(&cleanupTimer,                     SIGNAL(timeout()),                                                                            this, SLOT(cleanupTimerTicked()));
    connect(&userUpdateTimer,                  SIGNAL(timeout()),                                                                            this, SLOT(userUpdateTimerTicked()));
    connect(&priceCollectTimer,                SIGNAL(timeout()),                                                                            this, SLOT(priceCollectTimerTicked()));
    connect(&makeDecisionTimer,                SIGNAL(timeout()),                                                                            this, SLOT(makeDecisionTimerTicked()));
    connect(&stocksTableUpdateAllTimer,        SIGNAL(timeout()),                                                                            this, SLOT(stocksTableUpdateAllTimerTicked()));
    connect(&stocksTableUpdateLastPricesTimer, SIGNAL(timeout()),                                                                            this, SLOT(stocksTableUpdateLastPricesTimerTicked()));
    connect(&keepMoneyChangeDelayTimer,        SIGNAL(timeout()),                                                                            this, SLOT(keepMoneyChangeDelayTimerTicked()));
    connect(mPriceCollectThread,               SIGNAL(notifyInstrumentsProgress(const QString&)),                                            this, SLOT(notifyInstrumentsProgress(const QString&)));
    connect(mPriceCollectThread,               SIGNAL(stocksChanged()),                                                                      this, SLOT(stocksChanged()));
    connect(mPriceCollectThread,               SIGNAL(pricesChanged()),                                                                      this, SLOT(pricesChanged()));
    connect(mPriceCollectThread,               SIGNAL(periodicDataChanged()),                                                                this, SLOT(periodicDataChanged()));
    connect(mLastPriceThread,                  SIGNAL(lastPriceChanged(const QString&)),                                                     this, SLOT(lastPriceChanged(const QString&)));
    connect(mOperationsThread,                 SIGNAL(accountNotFound()),                                                                    this, SLOT(stopAutoPilot()));
    connect(mOperationsThread,                 SIGNAL(operationsRead(const QList<Operation>&)),                                              this, SLOT(autoPilotOperationsRead(const QList<Operation>&)));
    connect(mOperationsThread,                 SIGNAL(operationsAdded(const QList<Operation>&)),                                             this, SLOT(autoPilotOperationsAdded(const QList<Operation>&)));
    connect(mPortfolioThread,                  SIGNAL(accountNotFound()),                                                                    this, SLOT(stopAutoPilot()));
    connect(mPortfolioThread,                  SIGNAL(portfolioChanged(const Portfolio&)),                                                   this, SLOT(autoPilotPortfolioChanged(const Portfolio&)));
    connect(mPortfolioLastPriceThread,         SIGNAL(lastPriceChanged(const QString&, float)),                                              this, SLOT(autoPilotPortfolioLastPriceChanged(const QString&, float)));
    connect(mStocksControlsWidget,             SIGNAL(dateChangeDateTimeChanged(const QDateTime&)),                                          this, SLOT(dateChangeDateTimeChanged(const QDateTime&)));
    connect(mStocksControlsWidget,             SIGNAL(filterChanged(const Filter&)),                                                         this, SLOT(filterChanged(const Filter&)));
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
    mOperationsThread->terminateThread();
    mPortfolioThread->terminateThread();
    mPortfolioLastPriceThread->terminateThread();
    mMakeDecisionThread->terminateThread();

    mCleanupThread->wait();
    mUserUpdateThread->wait();
    mPriceCollectThread->wait();
    mLastPriceThread->wait();
    mOperationsThread->wait();
    mPortfolioThread->wait();
    mPortfolioLastPriceThread->wait();
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

    mUserUpdateThread->terminateThread();
    mPriceCollectThread->terminateThread();
    mLastPriceThread->terminateThread();
    mMakeDecisionThread->terminateThread();

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
    mMakeDecisionThread->wait();

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

void MainWindow::keepMoneyChangeDelayTimerTicked()
{
    mAutoPilotSettingsEditor->setValue("Options/KeepMoney", ui->keepMoneySpinBox->value());
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
    mStocksTableWidget->updatePrices(mStocksControlsWidget->getFilter());
}

void MainWindow::periodicDataChanged()
{
    mStocksTableWidget->updatePeriodicData(mStocksControlsWidget->getFilter());
}

void MainWindow::lastPriceChanged(const QString& instrumentId)
{
    mStocksTableWidget->lastPriceChanged(instrumentId);
}

void MainWindow::dateChangeDateTimeChanged(const QDateTime& dateTime)
{
    const QMutexLocker lock(mStocksStorage->getMutex());

    mStocksStorage->obtainStocksDatePrice(dateTime.toMSecsSinceEpoch());
    mStocksTableWidget->setDateChangeTooltip(tr("From: %1").arg(dateTime.toString(DATETIME_FORMAT)));
    mStocksTableWidget->updatePrices(mStocksControlsWidget->getFilter());
}

void MainWindow::filterChanged(const Filter& filter)
{
    mStocksTableWidget->filterChanged(filter);
}

void MainWindow::startSimulator() const
{
    ui->simulationActiveWidget->show();
    ui->simulationActiveSpinnerWidget->start();

    ui->startSimulationButton->setIcon(QIcon(":/assets/images/stop.png"));
    ui->startSimulationButton->setText(tr("Stop simulation"));

    // TODO: Start simulation
}

void MainWindow::stopSimulator() const
{
    ui->simulationActiveWidget->hide();
    ui->simulationActiveSpinnerWidget->stop();

    ui->startSimulationButton->setIcon(QIcon(":/assets/images/start.png"));
    ui->startSimulationButton->setText(tr("Start simulation"));

    // TODO: Stop simulation
}

void MainWindow::startAutoPilot() const
{
    ui->autoPilotActiveWidget->show();
    ui->autoPilotActiveSpinnerWidget->start();

    ui->startAutoPilotButton->setIcon(QIcon(":/assets/images/stop.png"));
    ui->startAutoPilotButton->setText(tr("Stop auto-pilot"));

    const QString account = mAutoPilotSettingsEditor->value("Options/Account", "").toString();

    mOperationsThread->setAccount(account);
    mPortfolioThread->setAccount(account);

    mOperationsThread->start();
    mPortfolioThread->start();
    mPortfolioLastPriceThread->start();
}

void MainWindow::stopAutoPilot() const
{
    ui->autoPilotActiveWidget->hide();
    ui->autoPilotActiveSpinnerWidget->stop();

    ui->startAutoPilotButton->setIcon(QIcon(":/assets/images/start.png"));
    ui->startAutoPilotButton->setText(tr("Start auto-pilot"));

    mOperationsThread->terminateThread();
    mPortfolioThread->terminateThread();
    mPortfolioLastPriceThread->terminateThread();

    mOperationsThread->wait();
    mPortfolioThread->wait();
    mPortfolioLastPriceThread->wait();
}

void MainWindow::autoPilotOperationsRead(const QList<Operation>& operations)
{
    mAutoPilotDecisionMakerWidget->operationsRead(operations);
}

void MainWindow::autoPilotOperationsAdded(const QList<Operation>& operations)
{
    mAutoPilotDecisionMakerWidget->operationsAdded(operations);
}

void MainWindow::autoPilotPortfolioChanged(const Portfolio& portfolio)
{
    mAutoPilotDecisionMakerWidget->portfolioChanged(portfolio);
}

void MainWindow::autoPilotPortfolioLastPriceChanged(const QString& instrumentId, float price)
{
    qInfo() << instrumentId << price;
}

void MainWindow::on_actionAuth_triggered()
{
    ui->actionAuth->setEnabled(false);

    userUpdateTimerTicked();
    priceCollectTimerTicked();
    makeDecisionTimerTicked();

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
            // clang-format off
            mSimulatorSettingsEditor->setValue("General/Enabled",    true);
            mSimulatorSettingsEditor->setValue("Options/StartMoney", dialog->startMoney());
            mSimulatorSettingsEditor->setValue("Options/Mode",       dialog->mode());
            mSimulatorSettingsEditor->setValue("Options/FromDate",   dialog->fromDate().toString(DATE_FORMAT));
            mSimulatorSettingsEditor->setValue("Options/ToDate",     dialog->toDate().toString(DATE_FORMAT));
            mSimulatorSettingsEditor->setValue("Options/BestConfig", dialog->bestConfig());
            // clang-format on

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
            mAutoPilotSettingsEditor->setValue("Options/Account",        dialog->account());
            mAutoPilotSettingsEditor->setValue("Options/Mode",           dialog->mode());
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

void MainWindow::on_keepMoneySpinBox_valueChanged(int /*value*/)
{
    keepMoneyChangeDelayTimer.start(KEEP_MONEY_CHANGE_DELAY);
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mUserStorage->readFromDatabase();
    mStocksStorage->readFromDatabase();
    mInstrumentsStorage->readFromDatabase();

    updateStocksTableWidget();

    cleanupTimer.start(CLEANUP_INTERVAL);
    cleanupTimerTicked();

    userUpdateTimer.setInterval(USER_UPDATE_INTERVAL);
    priceCollectTimer.setInterval(PRICE_COLLECT_INTERVAL);
    stocksTableUpdateAllTimer.setInterval(STOCKS_TABLE_UPDATE_ALL_INTERVAL);
    stocksTableUpdateLastPricesTimer.setInterval(STOCKS_TABLE_UPDATE_LAST_PRICES_INTERVAL);

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
    makeDecisionTimer.setInterval(mConfig->getMakeDecisionTimeout() * ONE_MINUTE);
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

    ui->keepMoneySpinBox->setValue(mAutoPilotSettingsEditor->value("Options/KeepMoney", 0).toInt());

    mStocksControlsWidget->loadWindowState("MainWindow/StocksControlsWidget");
    mStocksTableWidget->loadWindowState("MainWindow/StocksTableWidget");
    mSimulatorDecisionMakerWidget->loadWindowState("MainWindow/Simulator");
    mAutoPilotDecisionMakerWidget->loadWindowState("MainWindow/AutoPilot");

    updateStackWidgetToolbar();
}
