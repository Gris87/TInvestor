#include "trayicon.h"

#include <QMenu>
#include <QDebug>

#include "mainwindow.h"



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

    setIcon(QIcon(":/images/icon.png"));
    setContextMenu(trayIconMenu);
    setToolTip(tr("TInvestor"));

    QObject::connect(this, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), mainWindow, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));

    qDebug() << "Tray icon initialized";
}
