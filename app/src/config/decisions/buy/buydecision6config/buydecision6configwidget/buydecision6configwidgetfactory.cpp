#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/buydecision6configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/buydecision6configwidget.h"



BuyDecision6ConfigWidgetFactory::BuyDecision6ConfigWidgetFactory() :
    IBuyDecision6ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision6ConfigWidgetFactory";
}

BuyDecision6ConfigWidgetFactory::~BuyDecision6ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision6ConfigWidgetFactory";
}

IBuyDecision6ConfigWidget* BuyDecision6ConfigWidgetFactory::newInstance(IBuyDecision6Config* config, QWidget* parent) const
{
    return new BuyDecision6ConfigWidget(config, parent);
}
