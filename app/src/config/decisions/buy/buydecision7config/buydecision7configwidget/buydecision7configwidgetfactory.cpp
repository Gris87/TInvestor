#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/buydecision7configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/buydecision7configwidget.h"



BuyDecision7ConfigWidgetFactory::BuyDecision7ConfigWidgetFactory() :
    IBuyDecision7ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision7ConfigWidgetFactory";
}

BuyDecision7ConfigWidgetFactory::~BuyDecision7ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision7ConfigWidgetFactory";
}

IBuyDecision7ConfigWidget* BuyDecision7ConfigWidgetFactory::newInstance(IBuyDecision7Config* config, QWidget* parent) const
{
    return new BuyDecision7ConfigWidget(config, parent);
}
