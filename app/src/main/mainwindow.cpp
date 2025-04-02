#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QDir>
#include <QEvent>



#define AUTORUN_PATH "CurrentVersion/Run/TInvestor"



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
    ITableRecordFactory*               tableRecordFactory,
    IStocksControlsWidgetFactory*      stocksControlsWidgetFactory,
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
    authFailedDelayTimer(new QTimer(this)),
    userUpdateTimer(new QTimer(this)),
    priceCollectTimer(new QTimer(this)),
    cleanupTimer(new QTimer(this)),
    makeDecisionTimer(new QTimer(this)),
    stocksTableUpdateAllTimer(new QTimer(this)),
    stocksTableUpdatePriceTimer(new QTimer(this)),
    tableRecords(),
    lastPricesUpdates(),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mConfigForSimulation(configForSimulation),
    mAuthDialogFactory(authDialogFactory),
    mSettingsDialogFactory(settingsDialogFactory),
    mOrderWavesDialogFactory(orderWavesDialogFactory),
    mStartSimulationDialogFactory(startSimulationDialogFactory),
    mStartAutoPilotDialogFactory(startAutoPilotDialogFactory),
    mDecisionMakerConfigWidgetFactory(decisionMakerConfigWidgetFactory),
    mBuyDecision1ConfigWidgetFactory(buyDecision1ConfigWidgetFactory),
    mBuyDecision2ConfigWidgetFactory(buyDecision2ConfigWidgetFactory),
    mBuyDecision3ConfigWidgetFactory(buyDecision3ConfigWidgetFactory),
    mSellDecision1ConfigWidgetFactory(sellDecision1ConfigWidgetFactory),
    mSellDecision2ConfigWidgetFactory(sellDecision2ConfigWidgetFactory),
    mSellDecision3ConfigWidgetFactory(sellDecision3ConfigWidgetFactory),
    mStockTableItemWidgetFactory(stockTableItemWidgetFactory),
    mActionsTableItemWidgetFactory(actionsTableItemWidgetFactory),
    mOrderWavesWidgetFactory(orderWavesWidgetFactory),
    mTableRecordFactory(tableRecordFactory),
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

    ui->waitingSpinnerWidget->setColor(QColor("#AFC2D7"));
    ui->waitingSpinnerWidget->setTextColor(QColor("#AFC2D7"));

    mStocksControlsWidget         = stocksControlsWidgetFactory->newInstance(mStocksStorage, this);
    mSimulatorDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(mSettingsEditor, "Simulator", this);
    mAutoPilotDecisionMakerWidget = decisionMakerWidgetFactory->newInstance(mSettingsEditor, "AutoPilot", this);

    ui->layoutForStocksControlsWidget->addWidget(mStocksControlsWidget);
    ui->layoutForSimulatorDecisionMaker->addWidget(mSimulatorDecisionMakerWidget);
    ui->layoutForAutoPilotDecisionMaker->addWidget(mAutoPilotDecisionMakerWidget);

    mTrayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(mTrayIcon,                   SIGNAL(activated(QSystemTrayIcon::ActivationReason)),                         this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon,                   SIGNAL(trayIconShowClicked()),                                                this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon,                   SIGNAL(trayIconExitClicked()),                                                this, SLOT(trayIconExitClicked()));
    connect(mGrpcClient,                 SIGNAL(authFailed(grpc::StatusCode, const std::string&, const std::string&)), this, SLOT(authFailed(grpc::StatusCode, const std::string&, const std::string&)));
    connect(authFailedDelayTimer,        SIGNAL(timeout()),                                                            this, SLOT(authFailedDelayTimerTicked()));
    connect(userUpdateTimer,             SIGNAL(timeout()),                                                            this, SLOT(userUpdateTimerTicked()));
    connect(priceCollectTimer,           SIGNAL(timeout()),                                                            this, SLOT(priceCollectTimerTicked()));
    connect(cleanupTimer,                SIGNAL(timeout()),                                                            this, SLOT(cleanupTimerTicked()));
    connect(makeDecisionTimer,           SIGNAL(timeout()),                                                            this, SLOT(makeDecisionTimerTicked()));
    connect(stocksTableUpdateAllTimer,   SIGNAL(timeout()),                                                            this, SLOT(stocksTableUpdateAllTimerTicked()));
    connect(stocksTableUpdatePriceTimer, SIGNAL(timeout()),                                                            this, SLOT(stocksTableUpdatePriceTimerTicked()));
    connect(mPriceCollectThread,         SIGNAL(notifyStocksProgress(const QString&)),                                 this, SLOT(notifyStocksProgress(const QString&)));
    connect(mPriceCollectThread,         SIGNAL(stocksChanged()),                                                      this, SLOT(stocksChanged()));
    connect(mPriceCollectThread,         SIGNAL(pricesChanged()),                                                      this, SLOT(pricesChanged()));
    connect(mPriceCollectThread,         SIGNAL(periodicDataChanged()),                                                this, SLOT(periodicDataChanged()));
    connect(mLastPriceThread,            SIGNAL(lastPriceChanged(const QString&)),                                     this, SLOT(lastPriceChanged(const QString&)));
    connect(mStocksControlsWidget,       SIGNAL(dateChangeDateTimeChanged()),                                          this, SLOT(pricesChanged()));
    connect(mStocksControlsWidget,       SIGNAL(filterChanged(const Filter&)),                                         this, SLOT(filterChanged(const Filter&)));
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

    authFailedDelayTimer->start(3000); // 3 seconds
}

void MainWindow::authFailedDelayTimerTicked()
{
    qDebug() << "Authorization failed delay timer ticked";

    authFailedDelayTimer->stop();

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
    stocksTableUpdatePriceTimer->stop();

    ui->actionAuth->setEnabled(true);
    ui->waitingSpinnerWidget->setText(tr("Waiting for authorization"));
    trayIconShowClicked();

    std::shared_ptr<IAuthDialog> dialog = mAuthDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, this);

    if (dialog->exec())
    {
        mUserStorage->setToken(dialog->getToken());

        on_actionAuth_triggered();
    }
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

    const Filter& filter = mStocksControlsWidget->getFilter();

    ui->stocksTableWidget->setUpdatesEnabled(false);
    ui->stocksTableWidget->setSortingEnabled(false);

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
    {
        it.value()->updateAll();
        it.value()->filter(ui->stocksTableWidget, filter);
    }

    ui->stocksTableWidget->setSortingEnabled(true);
    ui->stocksTableWidget->setUpdatesEnabled(true);
}

void MainWindow::stocksTableUpdatePriceTimerTicked()
{
    qDebug() << "Stocks table update timer ticked";

    if (!lastPricesUpdates.isEmpty())
    {
        const Filter& filter = mStocksControlsWidget->getFilter();

        ui->stocksTableWidget->setUpdatesEnabled(false);
        ui->stocksTableWidget->setSortingEnabled(false);

        for (auto it = lastPricesUpdates.cbegin(); it != lastPricesUpdates.cend(); ++it)
        {
            ITableRecord* record = tableRecords[*it];

            if (record != nullptr)
            {
                record->updatePrice();
                record->filter(ui->stocksTableWidget, filter);
            }
        }

        lastPricesUpdates.clear();

        ui->stocksTableWidget->setSortingEnabled(true);
        ui->stocksTableWidget->setUpdatesEnabled(true);
    }
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
    const Filter& filter = mStocksControlsWidget->getFilter();

    ui->stocksTableWidget->setUpdatesEnabled(false);
    ui->stocksTableWidget->setSortingEnabled(false);

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
    {
        it.value()->updatePrice();
        it.value()->filter(ui->stocksTableWidget, filter);
    }

    ui->stocksTableWidget->setSortingEnabled(true);
    ui->stocksTableWidget->setUpdatesEnabled(true);
}

void MainWindow::periodicDataChanged()
{
    const Filter& filter = mStocksControlsWidget->getFilter();

    ui->stocksTableWidget->setUpdatesEnabled(false);
    ui->stocksTableWidget->setSortingEnabled(false);

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
    {
        it.value()->updatePeriodicData();
        it.value()->filter(ui->stocksTableWidget, filter);
    }

    ui->stocksTableWidget->setSortingEnabled(true);
    ui->stocksTableWidget->setUpdatesEnabled(true);
}

void MainWindow::lastPriceChanged(const QString& uid)
{
    lastPricesUpdates.insert(uid);
}

void MainWindow::filterChanged(const Filter& filter)
{
    ui->stocksTableWidget->setUpdatesEnabled(false);

    for (auto it = tableRecords.cbegin(); it != tableRecords.cend(); ++it)
    {
        it.value()->filter(ui->stocksTableWidget, filter);
    }

    ui->stocksTableWidget->setUpdatesEnabled(true);
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
    stocksTableUpdatePriceTimer->start();
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
    std::shared_ptr<IStartSimulationDialog> dialog = mStartSimulationDialogFactory->newInstance(mSettingsEditor, this);

    if (dialog->exec())
    {
        // TODO: Start simulation
    }
}

void MainWindow::on_startAutoPilotButton_clicked()
{
    std::shared_ptr<IStartAutoPilotDialog> dialog =
        mStartAutoPilotDialogFactory->newInstance(mUserStorage, mMessageBoxUtils, mSettingsEditor, this);

    if (dialog->exec())
    {
        // TODO: Start auto-pilot
    }
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mUserStorage->readFromDatabase();
    mStocksStorage->readFromDatabase();

    updateStocksTableWidget();
    ui->stocksTableWidget->sortByColumn(STOCK_COLUMN, Qt::AscendingOrder);

    userUpdateTimer->setInterval(15 * 60 * 1000);       // 15 minutes
    priceCollectTimer->setInterval(1 * 60 * 60 * 1000); // 1 hour

    cleanupTimer->start(24 * 60 * 60 * 1000); // 1 day
    cleanupTimerTicked();

    stocksTableUpdateAllTimer->setInterval(24 * 60 * 60 * 1000); // 1 day
    stocksTableUpdatePriceTimer->setInterval(3 * 1000);          // 3 seconds

    on_actionAuth_triggered();
}

void MainWindow::updateStocksTableWidget()
{
    QMutexLocker   lock(mStocksStorage->getMutex());
    QList<Stock*>& stocks = mStocksStorage->getStocks();

    if (!stocks.isEmpty())
    {
        const Filter& filter = mStocksControlsWidget->getFilter();

        ui->stocksTableWidget->setUpdatesEnabled(false);
        ui->stocksTableWidget->setSortingEnabled(false);

        for (int i = 0; i < stocks.size(); ++i)
        {
            Stock* stock = stocks.at(i);

            stock->mutex->lock();
            QString uid = stock->meta.uid;
            stock->mutex->unlock();

            ITableRecord* record = tableRecords[uid];

            if (record != nullptr)
            {
                record->updateAll();
            }
            else
            {
                record = mTableRecordFactory->newInstance(
                    ui->stocksTableWidget,
                    mStockTableItemWidgetFactory,
                    mActionsTableItemWidgetFactory,
                    mOrderWavesDialogFactory,
                    mOrderWavesWidgetFactory,
                    mUserStorage,
                    mOrderBookThread,
                    mHttpClient,
                    stock,
                    this
                );
                tableRecords[uid] = record;
            }

            record->filter(ui->stocksTableWidget, filter);
        }

        ui->stocksTableWidget->setSortingEnabled(true);
        ui->stocksTableWidget->setUpdatesEnabled(true);

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
    mSettingsEditor->setValue("MainWindow/geometry",                     saveGeometry());
    mSettingsEditor->setValue("MainWindow/windowState",                  saveState());
    mSettingsEditor->setValue("MainWindow/pageIndex",                    ui->stackedWidget->currentIndex());
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_Stock",      ui->stocksTableWidget->columnWidth(STOCK_COLUMN));
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_Price",      ui->stocksTableWidget->columnWidth(PRICE_COLUMN));
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_DayChange",  ui->stocksTableWidget->columnWidth(DAY_CHANGE_COLUMN));
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_DateChange", ui->stocksTableWidget->columnWidth(DATE_CHANGE_COLUMN));
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_Turnover",   ui->stocksTableWidget->columnWidth(TURNOVER_COLUMN));
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_Payback",    ui->stocksTableWidget->columnWidth(PAYBACK_COLUMN));
    mSettingsEditor->setValue("MainWindow/stocksTableWidget_Actions",    ui->stocksTableWidget->columnWidth(ACTIONS_COLUMN));
    // clang-format on

    mSimulatorDecisionMakerWidget->saveWindowState();
    mAutoPilotDecisionMakerWidget->saveWindowState();
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    restoreGeometry(mSettingsEditor->value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(mSettingsEditor->value("MainWindow/windowState", QByteArray()).toByteArray());
    ui->stackedWidget->setCurrentIndex(mSettingsEditor->value("MainWindow/pageIndex", 0).toInt());

    // clang-format off
    ui->stocksTableWidget->setColumnWidth(STOCK_COLUMN,       mSettingsEditor->value("MainWindow/stocksTableWidget_Stock",      99).toInt());
    ui->stocksTableWidget->setColumnWidth(PRICE_COLUMN,       mSettingsEditor->value("MainWindow/stocksTableWidget_Price",      61).toInt());
    ui->stocksTableWidget->setColumnWidth(DAY_CHANGE_COLUMN,  mSettingsEditor->value("MainWindow/stocksTableWidget_DayChange",  139).toInt());
    ui->stocksTableWidget->setColumnWidth(DATE_CHANGE_COLUMN, mSettingsEditor->value("MainWindow/stocksTableWidget_DateChange", 139).toInt());
    ui->stocksTableWidget->setColumnWidth(TURNOVER_COLUMN,    mSettingsEditor->value("MainWindow/stocksTableWidget_Turnover",   86).toInt());
    ui->stocksTableWidget->setColumnWidth(PAYBACK_COLUMN,     mSettingsEditor->value("MainWindow/stocksTableWidget_Payback",    120).toInt());
    ui->stocksTableWidget->setColumnWidth(ACTIONS_COLUMN,     mSettingsEditor->value("MainWindow/stocksTableWidget_Actions",    83).toInt());
    // clang-format on

    mSimulatorDecisionMakerWidget->loadWindowState();
    mAutoPilotDecisionMakerWidget->loadWindowState();

    updateStackWidgetToolbar();
}
