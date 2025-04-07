#include "src/widgets/trayicon/trayicon.h"

#include <QDebug>
#include <QMenu>

#include "src/main/mainwindow.h"



TrayIcon::TrayIcon(QObject* parent) :
    ITrayIcon(parent)
{
    qDebug() << "Create TrayIcon";

    QMenu* trayIconMenu = new QMenu(reinterpret_cast<QWidget*>(parent));

    QAction* defaultAction = trayIconMenu->addAction(tr("Show"), this, SIGNAL(trayIconShowClicked()));
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(tr("Exit"), this, SIGNAL(trayIconExitClicked()));

    trayIconMenu->setDefaultAction(defaultAction);

    setIcon(QIcon(":/assets/images/icon.png"));
    setContextMenu(trayIconMenu);
    setToolTip(tr("TInvestor"));
}

TrayIcon::~TrayIcon()
{
    qDebug() << "Destroy TrayIcon";
}
