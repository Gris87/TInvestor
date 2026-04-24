#include "src/widgets/trayicon/trayiconfactory.h"

#include <QDebug>

#include "src/widgets/trayicon/trayicon.h"



TrayIconFactory::TrayIconFactory() :
    ITrayIconFactory()
{
    qDebug() << "Create TrayIconFactory";
}

TrayIconFactory::~TrayIconFactory()
{
    qDebug() << "Destroy TrayIconFactory";
}

ITrayIcon* TrayIconFactory::newInstance(QObject* parent) const
{
    return new TrayIcon(parent);
}
