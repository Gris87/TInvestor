#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>

#include "src/config/settingsdialog.h"



MainWindow::MainWindow(
    IConfig *config,
    IConfig *configForSettingsDialog,
    ISettingsDialogFactory *settingsDialogFactory,
    IDecisionMakerConfigWidgetFactory *decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
    ITrayIconFactory *trayIconFactory,
    ISettingsEditor *settingsEditor,
    IStocksDatabase *stocksDatabase,
    ICleanupThread *cleanupThread,
    IRefreshThread *refreshThread
) :
    QMainWindow(),
    ui(new Ui::MainWindow),
    cleanupTimer(new QTimer(this)),
    refreshTimer(new QTimer(this)),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mSettingsDialogFactory(settingsDialogFactory),
    mDecisionMakerConfigWidgetFactory(decisionMakerConfigWidgetFactory),
    mBuyDecision1ConfigWidgetFactory(buyDecision1ConfigWidgetFactory),
    mBuyDecision2ConfigWidgetFactory(buyDecision2ConfigWidgetFactory),
    mBuyDecision3ConfigWidgetFactory(buyDecision3ConfigWidgetFactory),
    mSellDecision1ConfigWidgetFactory(sellDecision1ConfigWidgetFactory),
    mSellDecision2ConfigWidgetFactory(sellDecision2ConfigWidgetFactory),
    mSellDecision3ConfigWidgetFactory(sellDecision3ConfigWidgetFactory),
    mSettingsEditor(settingsEditor),
    mStocksDatabase(stocksDatabase),
    mCleanupThread(cleanupThread),
    mRefreshThread(refreshThread),
    mStocks()
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    ITrayIcon *trayIcon = trayIconFactory->newInstance(this);

    connect(trayIcon, SIGNAL(trayIconClicked(QSystemTrayIcon::ActivationReason reason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason reason)));
    connect(trayIcon, SIGNAL(trayIconShowClicked()), this, SLOT(trayIconShowClicked()));
    connect(trayIcon, SIGNAL(trayIconExitClicked()), this, SLOT(trayIconExitClicked()));

    trayIcon->show();

    connect(cleanupTimer, SIGNAL(timeout()), this, SLOT(cleanupTimerTicked()));
    connect(refreshTimer, SIGNAL(timeout()), this, SLOT(refreshTimerTicked()));

    mConfig->makeDefault();
    mConfig->load(mSettingsEditor);
    applyConfig();

    loadWindowState();
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroy MainWindow";

    mCleanupThread->requestInterruption();
    mRefreshThread->requestInterruption();

    mCleanupThread->wait();
    mRefreshThread->wait();

    saveWindowState();

    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (!event->spontaneous() || !isVisible())
    {
        qDebug() << "Application closed";

        return;
    }

    event->ignore();
    hide();

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

void MainWindow::cleanupTimerTicked()
{
    qInfo() << "Cleanup timer ticked";

    if (mRefreshThread->isRunning())
    {
        mRefreshThread->wait();
    }

    mCleanupThread->start();
}

void MainWindow::refreshTimerTicked()
{
    qDebug() << "Refresh timer ticked";

    if (mCleanupThread->isRunning() || mRefreshThread->isRunning())
    {
        return;
    }

    refreshTimer->start();
    mRefreshThread->start();
}

void MainWindow::on_actionRefreshManually_triggered()
{
    refreshTimerTicked();
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

    ISettingsDialog *dialog = mSettingsDialogFactory->newInstance(
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

    delete dialog;
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mStocks = mStocksDatabase->readStocks();

    cleanupTimer->start(24 * 60 * 60 * 1000); // 1 day
    cleanupTimerTicked();
    mCleanupThread->wait();

    refreshTimerTicked();
}

void MainWindow::updateStackWidgetToolbar()
{
    ui->actionStocksPage->setChecked(ui->stackedWidget->currentWidget() == ui->stocksPage);
    ui->actionSimulationPage->setChecked(ui->stackedWidget->currentWidget() == ui->simulationPage);
    ui->actionAutoPilotPage->setChecked(ui->stackedWidget->currentWidget() == ui->autoPilotPage);
}

void MainWindow::applyConfig()
{
    refreshTimer->setInterval(mConfig->getRefreshTimeout() * 60 * 1000);
}

void MainWindow::saveWindowState()
{
    qDebug() << "Saving window state";

    mSettingsEditor->setValue("MainWindow/geometry",    saveGeometry());
    mSettingsEditor->setValue("MainWindow/windowState", saveState());
    mSettingsEditor->setValue("MainWindow/pageIndex",   ui->stackedWidget->currentIndex());
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    restoreGeometry(mSettingsEditor->value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(mSettingsEditor->value("MainWindow/windowState", QByteArray()).toByteArray());
    ui->stackedWidget->setCurrentIndex(mSettingsEditor->value("MainWindow/pageIndex", 0).toInt());

    updateStackWidgetToolbar();
}
