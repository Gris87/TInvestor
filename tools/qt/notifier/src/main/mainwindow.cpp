#include "src/main/mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>



#ifdef Q_OS_WINDOWS
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::DoubleClick;
#else
constexpr QSystemTrayIcon::ActivationReason DOUBLE_CLICK_REASON = QSystemTrayIcon::Trigger;
#endif



MainWindow::MainWindow(ITrayIconFactory* trayIconFactory, QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
    qDebug() << "Destroy MainWindow";

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

void MainWindow::init()
{
    qInfo() << "Start main initialization";
}
