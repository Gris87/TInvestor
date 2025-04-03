#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QEvent>



#define AUTORUN_PATH    "CurrentVersion/Run/TInvestor"
#define GREY_COLOR      QColor("#AFC2D7")
#define DATETIME_FORMAT "yyyy-MM-dd hh:mm:ss"



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
    ISettingsEditor*                   autorunSettingsEditor
) :
    QMainWindow(),
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
    mAutorunSettingsEditor(autorunSettingsEditor)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    ui->waitingSpinnerWidget->setColor(GREY_COLOR);
    ui->waitingSpinnerWidget->setTextColor(GREY_COLOR);

    ui->simulatorActiveWidget->setVisible(false);
    ui->simulatorActiveSpinnerWidget->setInnerRadius(6);
    ui->simulatorActiveSpinnerWidget->setLineLength(6);
    ui->simulatorActiveSpinnerWidget->setColor(GREY_COLOR);

    ui->autoPilotActiveWidget->setVisible(false);
    ui->autoPilotActiveSpinnerWidget->setInnerRadius(6);
    ui->autoPilotActiveSpinnerWidget->setLineLength(6);
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
    connect(mTrayIcon,                        SIGNAL(activated(QSystemTrayIcon::ActivationReason)),                         this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon,                        SIGNAL(trayIconShowClicked()),                                                this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon,                        SIGNAL(trayIconExitClicked()),                                                this, SLOT(trayIconExitClicked()));
    connect(mGrpcClient,                      SIGNAL(authFailed(grpc::StatusCode, const std::string&, const std::string&)), this, SLOT(authFailed(grpc::StatusCode, const std::string&, const std::string&)));
    connect(userUpdateTimer,                  SIGNAL(timeout()),                                                            this, SLOT(userUpdateTimerTicked()));
    connect(priceCollectTimer,                SIGNAL(timeout()),                                                            this, SLOT(priceCollectTimerTicked()));
    connect(cleanupTimer,                     SIGNAL(timeout()),                                                            this, SLOT(cleanupTimerTicked()));
    connect(makeDecisionTimer,                SIGNAL(timeout()),                                                            this, SLOT(makeDecisionTimerTicked()));
    connect(stocksTableUpdateAllTimer,        SIGNAL(timeout()),                                                            this, SLOT(stocksTableUpdateAllTimerTicked()));
    connect(stocksTableUpdateLastPricesTimer, SIGNAL(timeout()),                                                            this, SLOT(stocksTableUpdateLastPricesTimerTicked()));
    connect(mPriceCollectThread,              SIGNAL(notifyStocksProgress(const QString&)),                                 this, SLOT(notifyStocksProgress(const QString&)));
    connect(mPriceCollectThread,              SIGNAL(stocksChanged()),                                                      this, SLOT(stocksChanged()));
    connect(mPriceCollectThread,              SIGNAL(pricesChanged()),                                                      this, SLOT(pricesChanged()));
    connect(mPriceCollectThread,              SIGNAL(periodicDataChanged()),                                                this, SLOT(periodicDataChanged()));
    connect(mLastPriceThread,                 SIGNAL(lastPriceChanged(const QString&)),                                     this, SLOT(lastPriceChanged(const QString&)));
    connect(mStocksControlsWidget,            SIGNAL(dateChangeDateTimeChanged(const QDateTime&)),                          this, SLOT(dateChangeDateTimeChanged(const QDateTime&)));
    connect(mStocksControlsWidget,            SIGNAL(filterChanged(const Filter&)),                                         this, SLOT(filterChanged(const Filter&)));
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

    mTrayIcon->showMessage(tr("TInvestor"), tr("TInvestor is working in background"));

    qDebug() << "Main window moved to tray";
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick)
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

void MainWindow::authFailed(grpc::StatusCode errorCode, const std::string& errorMessage, const std::string& errorDetails)
{
    qWarning() << "Authorization failed with code:" << errorCode << "message:" << QString::fromStdString(errorMessage)
               << "details:" << QString::fromStdString(errorDetails);

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

    std::shared_ptr<IAuthDialog> dialog = mAuthDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, this);

    if (dialog->exec())
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

void MainWindow::notifyStocksProgress(const QString& message)
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

    std::shared_ptr<ISettingsDialog> dialog = mSettingsDialogFactory->newInstance(
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

    if (dialog->exec())
    {
        qInfo() << "Settings applied";

        mConfig->assign(mConfigForSettingsDialog);
        mConfig->save(mSettingsEditor);

        applyConfig();
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
    if (!ui->simulatorActiveWidget->isVisible())
    {
        std::shared_ptr<IStartSimulationDialog> dialog = mStartSimulationDialogFactory->newInstance(mSettingsEditor, this);

        if (dialog->exec())
        {
            ui->simulatorActiveWidget->setVisible(true);
            ui->simulatorActiveSpinnerWidget->start();

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
            ui->simulatorActiveWidget->setVisible(false);
            ui->simulatorActiveSpinnerWidget->stop();

            ui->startSimulationButton->setIcon(QIcon(":/assets/images/start.png"));
            ui->startSimulationButton->setText(tr("Start simulation"));

            // TODO: Stop simulation
        }
    }
}

void MainWindow::on_startAutoPilotButton_clicked()
{
    if (!ui->autoPilotActiveWidget->isVisible())
    {
        std::shared_ptr<IStartAutoPilotDialog> dialog =
            mStartAutoPilotDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, mSettingsEditor, this);

        if (dialog->exec())
        {
            ui->autoPilotActiveWidget->setVisible(true);
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
            ui->autoPilotActiveWidget->setVisible(false);
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

    userUpdateTimer->setInterval(15 * 60 * 1000);       // 15 minutes
    priceCollectTimer->setInterval(1 * 60 * 60 * 1000); // 1 hour

    cleanupTimer->start(24 * 60 * 60 * 1000); // 1 day
    cleanupTimerTicked();

    stocksTableUpdateAllTimer->setInterval(24 * 60 * 60 * 1000); // 1 day
    stocksTableUpdateLastPricesTimer->setInterval(3 * 1000);     // 3 seconds

    on_actionAuth_triggered();
}

void MainWindow::updateStocksTableWidget()
{
    QMutexLocker   lock(mStocksStorage->getMutex());
    QList<Stock*>& stocks = mStocksStorage->getStocks();

    if (!stocks.isEmpty())
    {
        QDateTime dateChangeTime = mStocksControlsWidget->getDateChangeTime();

        mStocksStorage->obtainStocksDatePrice(dateChangeTime.toMSecsSinceEpoch());
        mStocksTableWidget->setDateChangeTooltip(tr("From: %1").arg(dateChangeTime.toString(DATETIME_FORMAT)));
        mStocksTableWidget->updateTable(stocks, mStocksControlsWidget->getFilter());

        ui->stackedWidget->setVisible(true);
        ui->waitingSpinnerWidget->setVisible(false);
        ui->waitingSpinnerWidget->stop();
    }
    else
    {
        ui->stackedWidget->setVisible(false);
        ui->waitingSpinnerWidget->setVisible(true);
        ui->waitingSpinnerWidget->start();
    }
}

void MainWindow::updateStackWidgetToolbar()
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
    makeDecisionTimer->setInterval(mConfig->getMakeDecisionTimeout() * 60 * 1000);

    if (mConfig->isAutorun())
    {
        QString appPath = QDir::toNativeSeparators(qApp->applicationFilePath());

        mAutorunSettingsEditor->setValue(AUTORUN_PATH, QString("\"%1\" --autorun").arg(appPath));
    }
    else
    {
        mAutorunSettingsEditor->remove(AUTORUN_PATH);
    }
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
