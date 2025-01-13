#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>
#include <QSettings>

#include "src/config/settingsdialog.h"



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mConfig(this)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    mTrayIcon = new TrayIcon(this);
    mTrayIcon->init(this);
    mTrayIcon->show();

    mConfig.load();

    loadWindowState();
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroy MainWindow";

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

}

void MainWindow::on_actionFilterStocks_toggled(bool /*checked*/)
{

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
    SettingsDialog dialog(mConfig, this);

    if (dialog.exec())
    {
        qDebug() << "Settings applied";

        mConfig = dialog.getConfig();
        mConfig.save();
    }
}

void MainWindow::init()
{
    qDebug() << "Start main initialization";
}

void MainWindow::updateStackWidgetToolbar()
{
    ui->actionStocksPage->setChecked(ui->stackedWidget->currentWidget() == ui->stocksPage);
    ui->actionSimulationPage->setChecked(ui->stackedWidget->currentWidget() == ui->simulationPage);
    ui->actionAutoPilotPage->setChecked(ui->stackedWidget->currentWidget() == ui->autoPilotPage);
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
