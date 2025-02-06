#include "trayiconfactory.h"

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

ITrayIcon* TrayIconFactory::newInstance(QWidget *parent)
{
    return new TrayIcon(parent);
}
