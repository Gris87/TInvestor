#include "trayiconfactory.h"

#include <QDebug>

#include "src/main/trayicon.h"



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
