#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>



#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



MainWindow::MainWindow(
    IConfig*                config,
    IConfig*                configForSettingsDialog,
    ISettingsDialogFactory* settingsDialogFactory,
    ITrayIconFactory*       trayIconFactory,
    ISettingsEditor*        settingsEditor,
    IAutorunEnabler*        autorunEnabler,
    QWidget*                parent
) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mSettingsDialogFactory(settingsDialogFactory),
    mSettingsEditor(settingsEditor),
    mAutorunEnabler(autorunEnabler)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    mTrayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(mTrayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon, SIGNAL(trayIconShowClicked()),                        this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon, SIGNAL(trayIconExitClicked()),                        this, SLOT(trayIconExitClicked()));
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

void MainWindow::on_actionSettings_triggered()
{
    mConfigForSettingsDialog->assign(mConfig);

    const std::shared_ptr<ISettingsDialog> dialog = mSettingsDialogFactory->newInstance(mConfigForSettingsDialog, this);

    dialog->updateUiFromConfig();

    if (dialog->exec() == QDialog::Accepted)
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
}

void MainWindow::applyConfig()
{
    mAutorunEnabler->setEnabled(mConfig->isAutorun());
}

void MainWindow::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("MainWindow/geometry",    saveGeometry());
    mSettingsEditor->setValue("MainWindow/windowState", saveState());
    // clang-format on
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    restoreGeometry(mSettingsEditor->value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(mSettingsEditor->value("MainWindow/windowState", QByteArray()).toByteArray());
}
