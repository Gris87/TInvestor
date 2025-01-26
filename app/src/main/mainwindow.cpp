#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>
#include <QSettings>

#include "src/config/settingsdialog.h"



MainWindow::MainWindow(IConfigFactory *configFactory, IDecisionMakerConfigFactory *decisionMakerConfigFactory, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mConfig(configFactory->newInstance(decisionMakerConfigFactory, this)),
    mConfigForSettingsDialog(configFactory->newInstance(decisionMakerConfigFactory, this)),
    mCleanupTimer(new QTimer(this)),
    mRefreshTimer(new QTimer(this)),
    mCleanupThread(new CleanupThread(this)),
    mRefreshThread(new RefreshThread(this)),
    mStocksDatabase(new StocksDatabase(this)),
    mStocks()
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    mTrayIcon = new TrayIcon(this);
    mTrayIcon->init(this);
    mTrayIcon->show();

    connect(mCleanupTimer, SIGNAL(timeout()), this, SLOT(cleanupTimerTicked()));
    connect(mRefreshTimer, SIGNAL(timeout()), this, SLOT(refreshTimerTicked()));

    mConfig->load();
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
    switch (reason)
    {
        case QSystemTrayIcon::DoubleClick:
        {
            qDebug() << "Tray icon double clicked";

            trayIconShowClicked();
        }
        break;

        case QSystemTrayIcon::Context:
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::MiddleClick:
        {
            // Nothing
        }
        break;

        default:
        {
            qFatal("Unknown activation reason %u", reason);
        }
        break;
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

    SettingsDialog dialog(mConfigForSettingsDialog, this);

    if (dialog.exec())
    {
        qInfo() << "Settings applied";

        mConfig->assign(mConfigForSettingsDialog);
        mConfig->save();

        applyConfig();
    }
}

void MainWindow::cleanupTimerTicked()
{
    qInfo() << "Cleanup timer ticked";

    mCleanupThread->start();
}

void MainWindow::refreshTimerTicked()
{
    qDebug() << "Refresh timer ticked";

    if (mCleanupThread->isRunning() || mRefreshThread->isRunning())
    {
        return;
    }

    mRefreshTimer->start();
    mRefreshThread->start();
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mStocksDatabase->readStocks(&mStocks);

    mCleanupTimer->start(24 * 60 * 60 * 1000); // 1 day
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
    mRefreshTimer->setInterval(mConfig->getRefreshTimeout() * 60 * 1000);
}

void MainWindow::saveWindowState()
{
    qDebug() << "Saving window state";

    QSettings settings("GrisCom", "TInvestor");

    settings.setValue("MainWindow/geometry",    saveGeometry());
    settings.setValue("MainWindow/windowState", saveState());
    settings.setValue("MainWindow/pageIndex",   ui->stackedWidget->currentIndex());
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    QSettings settings("GrisCom", "TInvestor");

    restoreGeometry(settings.value("MainWindow/geometry").toByteArray());
    restoreState(settings.value("MainWindow/windowState").toByteArray());
    ui->stackedWidget->setCurrentIndex(settings.value("MainWindow/pageIndex").toInt());

    updateStackWidgetToolbar();
}
