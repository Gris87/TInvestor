#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/buydecision5configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/buydecision5configwidget.h"



BuyDecision5ConfigWidgetFactory::BuyDecision5ConfigWidgetFactory() :
    IBuyDecision5ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision5ConfigWidgetFactory";
}

BuyDecision5ConfigWidgetFactory::~BuyDecision5ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision5ConfigWidgetFactory";
}

IBuyDecision5ConfigWidget* BuyDecision5ConfigWidgetFactory::newInstance(IBuyDecision5Config* config, QWidget* parent) const
{
    return new BuyDecision5ConfigWidget(config, parent);
}
