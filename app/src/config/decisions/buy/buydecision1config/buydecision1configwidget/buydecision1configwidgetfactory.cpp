#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidget.h"



BuyDecision1ConfigWidgetFactory::BuyDecision1ConfigWidgetFactory() :
    IBuyDecision1ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision1ConfigWidgetFactory";
}

BuyDecision1ConfigWidgetFactory::~BuyDecision1ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision1ConfigWidgetFactory";
}

IBuyDecision1ConfigWidget* BuyDecision1ConfigWidgetFactory::newInstance(IBuyDecision1Config* config, QWidget* parent) const
{
    return new BuyDecision1ConfigWidget(config, parent);
}
