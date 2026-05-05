#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>



constexpr qint64 MS_IN_SECOND                = 1000LL;
constexpr qint64 ONE_MINUTE                  = 60LL * MS_IN_SECOND;
constexpr qint64 REQUEST_INTERVAL            = ONE_MINUTE; // 1 minute
constexpr qint64 REFRESH_BACKGROUND_INTERVAL = ONE_MINUTE; // 1 minute

#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



MainWindow::MainWindow(
    IConfig*                          config,
    IConfig*                          configForSettingsDialog,
    ISettingsDialogFactory*           settingsDialogFactory,
    INotificationsTableWidgetFactory* notificationsTableWidgetFactory,
    INotificationsTableModelFactory*  notificationsTableModelFactory,
    ITrayIconFactory*                 trayIconFactory,
    INotifier*                        notifier,
    IRequestThread*                   requestThread,
    ISettingsEditor*                  settingsEditor,
    IAutorunEnabler*                  autorunEnabler,
    QWidget*                          parent
) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    mConfig(config),
    mConfigForSettingsDialog(configForSettingsDialog),
    mSettingsDialogFactory(settingsDialogFactory),
    mNotifier(notifier),
    mRequestThread(requestThread),
    mSettingsEditor(settingsEditor),
    mAutorunEnabler(autorunEnabler)
{
    qDebug() << "Create MainWindow";

    ui->setupUi(this);

    mNotificationsTableWidget =
        notificationsTableWidgetFactory->newInstance(notificationsTableModelFactory, mSettingsEditor, this);
    ui->layoutForNotificationsTableWidget->addWidget(mNotificationsTableWidget);

    mTrayIcon = trayIconFactory->newInstance(this);

    // clang-format off
    connect(mTrayIcon,               SIGNAL(activated(QSystemTrayIcon::ActivationReason)),       this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
    connect(mTrayIcon,               SIGNAL(trayIconShowClicked()),                              this, SLOT(trayIconShowClicked()));
    connect(mTrayIcon,               SIGNAL(trayIconExitClicked()),                              this, SLOT(trayIconExitClicked()));
    connect(&requestTimer,           SIGNAL(timeout()),                                          this, SLOT(requestTimerTicked()));
    connect(&refreshBackgroundTimer, SIGNAL(timeout()),                                          this, SLOT(refreshBackgroundTimerTicked()));
    connect(mNotifier,               SIGNAL(notificationClicked()),                              this, SLOT(trayIconShowClicked()));
    connect(mRequestThread,          SIGNAL(notificationsRead(const QList<NotificationInfo>&)),  this, SLOT(notificationsRead(const QList<NotificationInfo>&)));
    connect(mRequestThread,          SIGNAL(notificationsAdded(const QList<NotificationInfo>&)), this, SLOT(notificationsAdded(const QList<NotificationInfo>&)));
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

    mRequestThread->terminateThread();
    mRequestThread->wait();

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

    mTrayIcon->resetCounter();
    mNotifier->resetNotificationWidgets();

    qDebug() << "Main window displayed";
}

void MainWindow::trayIconExitClicked()
{
    QCoreApplication::quit();
}

void MainWindow::requestTimerTicked()
{
    mRequestThread->start();
}

void MainWindow::refreshBackgroundTimerTicked()
{
    mNotificationsTableWidget->refreshBackground();
}

void MainWindow::notificationsRead(const QList<NotificationInfo>& notifications)
{
    mNotificationsTableWidget->notificationsRead(notifications);
}

void MainWindow::notificationsAdded(const QList<NotificationInfo>& notifications)
{
    mNotificationsTableWidget->notificationsAdded(notifications);

    if (!isVisible())
    {
        mTrayIcon->notificationsAdded(notifications);
        mNotifier->notificationsAdded(notifications);
    }
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

    requestTimer.start(REQUEST_INTERVAL);
    refreshBackgroundTimer.start(REFRESH_BACKGROUND_INTERVAL);

    requestTimerTicked();
    refreshBackgroundTimerTicked();
}

void MainWindow::applyConfig()
{
    mAutorunEnabler->setEnabled(mConfig->isAutorun());
    mNotifier->setEnabled(mConfig->isNotificationsEnabled());

    QList<MessageType> types;

    prepareFilterTypes(types, mConfig->isFilterSystem(), MESSAGE_TYPE_SYSTEM);
    prepareFilterTypes(types, mConfig->isFilterPortfolio(), MESSAGE_TYPE_PORTFOLIO);
    prepareFilterTypes(types, mConfig->isFilterHugeSell(), MESSAGE_TYPE_HUGE_SELL);
    prepareFilterTypes(types, mConfig->isFilterDividends(), MESSAGE_TYPE_DIVIDENDS);
    prepareFilterTypes(types, mConfig->isFilterPulse(), MESSAGE_TYPE_PULSE);

    Filter filter;
    filter.setMessageTypeMask(types);

    mNotificationsTableWidget->setFilter(filter);
    mTrayIcon->setFilter(filter);
    mNotifier->setFilter(filter);
}

void MainWindow::prepareFilterTypes(QList<MessageType>& types, bool enabled, MessageType messageType)
{
    if (enabled)
    {
        types.append(messageType);
    }
}

void MainWindow::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("MainWindow/geometry",    saveGeometry());
    mSettingsEditor->setValue("MainWindow/windowState", saveState());
    // clang-format on

    mNotificationsTableWidget->saveWindowState("MainWindow/NotificationsTableWidget");
}

void MainWindow::loadWindowState()
{
    qDebug() << "Loading window state";

    restoreGeometry(mSettingsEditor->value("MainWindow/geometry", QByteArray()).toByteArray());
    restoreState(mSettingsEditor->value("MainWindow/windowState", QByteArray()).toByteArray());

    mNotificationsTableWidget->loadWindowState("MainWindow/NotificationsTableWidget");
}
