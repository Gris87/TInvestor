#include "trayicon.h"

#include <QDebug>
#include <QMenu>

#include "src/main/mainwindow.h"



TrayIcon::TrayIcon(QObject *parent) :
    QSystemTrayIcon(parent)
{
    qDebug() << "Create TrayIcon";
}

TrayIcon::~TrayIcon()
{
    qDebug() << "Destroy TrayIcon";
}

void TrayIcon::init(MainWindow *mainWindow)
{
    QMenu *trayIconMenu = new QMenu(mainWindow);

    QAction *defaultAction = trayIconMenu->addAction(tr("Show"), mainWindow, SLOT(trayIconShowClicked()));
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("Exit"), mainWindow, SLOT(trayIconExitClicked()));

    trayIconMenu->setDefaultAction(defaultAction);

    setIcon(QIcon(":/assets/images/icon.png"));
    setContextMenu(trayIconMenu);
    setToolTip(tr("TInvestor"));

    connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), mainWindow, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

    qDebug() << "Tray icon initialized";
}
