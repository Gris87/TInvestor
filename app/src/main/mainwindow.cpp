#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>

#include "src/config/settingsdialog/settingsdialog.h"
#include "src/utils/objectholder/objectholder.h"



MainWindow::MainWindow(
    IConfig*                           config,
    IConfig*                           configForSettingsDialog,
    IConfig*                           configForSimulation,
    ISettingsDialogFactory*            settingsDialogFactory,
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    ITrayIconFactory*                  trayIconFactory,
    ISettingsEditor*                   settingsEditor,
    IUserDatabase*                     userDatabase,
    IUserStorage*                      userStorage,
    IStocksDatabase*                   stocksDatabase,
    IStocksStorage*                    stocksStorage,
    ICleanupThread*                    cleanupThread,
    IMakeDecisionThread*               makeDecisionThread
) :
    QMainWindow(),
    ui(new Ui::MainWindow),
    cleanupTimer(new QTimer(this)),
    makeDecisionTimer(new QTimer(this)),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mConfigForSimulation(configForSimulation),
    mSettingsDialogFactory(settingsDialogFactory),
    mDecisionMakerConfigWidgetFactory(decisionMakerConfigWidgetFactory),
    mBuyDecision1ConfigWidgetFactory(buyDecision1ConfigWidgetFactory),
    mBuyDecision2ConfigWidgetFactory(buyDecision2ConfigWidgetFactory),
    mBuyDecision3ConfigWidgetFactory(buyDecision3ConfigWidgetFactory),
    mSellDecision1ConfigWidgetFactory(sellDecision1ConfigWidgetFactory),
    mSellDecision2ConfigWidgetFactory(sellDecision2ConfigWidgetFactory),
    mSellDecision3ConfigWidgetFactory(sellDecision3ConfigWidgetFactory),
    mSettingsEditor(settingsEditor),
    mUserDatabase(userDatabase),
    mUserStorage(userStorage),
    mStocksDatabase(stocksDatabase),
    mStocksStorage(stocksStorage),
    mCleanupThread(cleanupThread),
    mMakeDecisionThread(makeDecisionThread)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    ITrayIcon* trayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(trayIcon, SIGNAL(trayIconShowClicked()),                        this, SLOT(trayIconShowClicked()));
    connect(trayIcon, SIGNAL(trayIconExitClicked()),                        this, SLOT(trayIconExitClicked()));
    // clang-format on

    trayIcon->show();

    // clang-format off
    connect(cleanupTimer,      SIGNAL(timeout()), this, SLOT(cleanupTimerTicked()));
    connect(makeDecisionTimer, SIGNAL(timeout()), this, SLOT(makeDecisionTimerTicked()));
    // clang-format on

    mConfig->makeDefault();
    mConfig->load(mSettingsEditor);
    applyConfig();

    loadWindowState();
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroy MainWindow";

    mCleanupThread->requestInterruption();
    mMakeDecisionThread->requestInterruption();

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

    mCleanupThread->start();
}

void MainWindow::makeDecisionTimerTicked()
{
    qDebug() << "Make decision timer ticked";

    mMakeDecisionThread->start();
}

void MainWindow::on_actionAuth_triggered()
{
    ui->actionAuth->setEnabled(false);
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

    ISettingsDialog* dialog = mSettingsDialogFactory->newInstance(
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
    ObjectHolder objectHolder(dialog);

    dialog->updateUiFromConfig();

    if (dialog->exec())
    {
        qInfo() << "Settings applied";

        mConfig->assign(mConfigForSettingsDialog);
        mConfig->save(mSettingsEditor);

        applyConfig();
    }
}

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mUserStorage->readFromDatabase(mUserDatabase);
    mStocksStorage->readFromDatabase(mStocksDatabase);

    cleanupTimer->start(24 * 60 * 60 * 1000); // 1 day
    cleanupTimerTicked();
    mCleanupThread->wait();

    on_actionAuth_triggered();
}

void MainWindow::updateStackWidgetToolbar()
{
    ui->actionStocksPage->setChecked(ui->stackedWidget->currentWidget() == ui->stocksPage);
    ui->actionSimulationPage->setChecked(ui->stackedWidget->currentWidget() == ui->simulationPage);
    ui->actionAutoPilotPage->setChecked(ui->stackedWidget->currentWidget() == ui->autoPilotPage);
}

void MainWindow::applyConfig()
{
    makeDecisionTimer->setInterval(mConfig->getMakeDecisionTimeout() * 60 * 1000);
}

void MainWindow::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("MainWindow/geometry",    saveGeometry());
    mSettingsEditor->setValue("MainWindow/windowState", saveState());
    mSettingsEditor->setValue("MainWindow/pageIndex",   ui->stackedWidget->currentIndex());
    // clang-format on
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    restoreGeometry(mSettingsEditor->value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(mSettingsEditor->value("MainWindow/windowState", QByteArray()).toByteArray());
    ui->stackedWidget->setCurrentIndex(mSettingsEditor->value("MainWindow/pageIndex", 0).toInt());

    updateStackWidgetToolbar();
}
