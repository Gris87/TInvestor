#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/buydecision8configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/buydecision8configwidget.h"



BuyDecision8ConfigWidgetFactory::BuyDecision8ConfigWidgetFactory() :
    IBuyDecision8ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision8ConfigWidgetFactory";
}

BuyDecision8ConfigWidgetFactory::~BuyDecision8ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision8ConfigWidgetFactory";
}

IBuyDecision8ConfigWidget* BuyDecision8ConfigWidgetFactory::newInstance(IBuyDecision8Config* config, QWidget* parent) const
{
    return new BuyDecision8ConfigWidget(config, parent);
}
