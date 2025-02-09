#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidget.h"



BuyDecision3ConfigWidgetFactory::BuyDecision3ConfigWidgetFactory() :
    IBuyDecision3ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision3ConfigWidgetFactory";
}

BuyDecision3ConfigWidgetFactory::~BuyDecision3ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision3ConfigWidgetFactory";
}

IBuyDecision3ConfigWidget* BuyDecision3ConfigWidgetFactory::newInstance(IBuyDecision3Config* config, QWidget* parent)
{
    return new BuyDecision3ConfigWidget(config, parent);
}
