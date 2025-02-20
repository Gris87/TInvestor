#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QEvent>



MainWindow::MainWindow(
    IConfig*                           config,
    IConfig*                           configForSettingsDialog,
    IConfig*                           configForSimulation,
    IAuthDialogFactory*                authDialogFactory,
    ISettingsDialogFactory*            settingsDialogFactory,
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    ITrayIconFactory*                  trayIconFactory,
    IUserStorage*                      userStorage,
    IStocksStorage*                    stocksStorage,
    IGrpcClient*                       grpcClient,
    IUserUpdateThread*                 userUpdateThread,
    ICleanupThread*                    cleanupThread,
    IMakeDecisionThread*               makeDecisionThread,
    IMessageBoxUtils*                  messageBoxUtils,
    ISettingsEditor*                   settingsEditor
) :
    QMainWindow(),
    ui(new Ui::MainWindow),
    userUpdateTimer(new QTimer(this)),
    cleanupTimer(new QTimer(this)),
    makeDecisionTimer(new QTimer(this)),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mConfigForSimulation(configForSimulation),
    mAuthDialogFactory(authDialogFactory),
    mSettingsDialogFactory(settingsDialogFactory),
    mDecisionMakerConfigWidgetFactory(decisionMakerConfigWidgetFactory),
    mBuyDecision1ConfigWidgetFactory(buyDecision1ConfigWidgetFactory),
    mBuyDecision2ConfigWidgetFactory(buyDecision2ConfigWidgetFactory),
    mBuyDecision3ConfigWidgetFactory(buyDecision3ConfigWidgetFactory),
    mSellDecision1ConfigWidgetFactory(sellDecision1ConfigWidgetFactory),
    mSellDecision2ConfigWidgetFactory(sellDecision2ConfigWidgetFactory),
    mSellDecision3ConfigWidgetFactory(sellDecision3ConfigWidgetFactory),
    mUserStorage(userStorage),
    mStocksStorage(stocksStorage),
    mGrpcClient(grpcClient),
    mUserUpdateThread(userUpdateThread),
    mCleanupThread(cleanupThread),
    mMakeDecisionThread(makeDecisionThread),
    mMessageBoxUtils(messageBoxUtils),
    mSettingsEditor(settingsEditor)
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

    connect(mGrpcClient, SIGNAL(authFailed()), this, SLOT(authFailed()));

    // clang-format off
    connect(userUpdateTimer,   SIGNAL(timeout()), this, SLOT(userUpdateTimerTicked()));
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

    mUserUpdateThread->requestInterruption();
    mCleanupThread->requestInterruption();
    mMakeDecisionThread->requestInterruption();

    mUserUpdateThread->wait();
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

void MainWindow::authFailed()
{
    qWarning() << "Authorization failed";

    ui->actionAuth->setEnabled(true);
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

    userUpdateTimerTicked();
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

void MainWindow::init()
{
    qInfo() << "Start main initialization";

    mUserStorage->readFromDatabase();
    mStocksStorage->readFromDatabase();

    userUpdateTimer->start(15 * 60 * 1000);   // 15 minutes
    cleanupTimer->start(24 * 60 * 60 * 1000); // 1 day
    cleanupTimerTicked();

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
