#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/buydecision9configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/buydecision9configwidget.h"



BuyDecision9ConfigWidgetFactory::BuyDecision9ConfigWidgetFactory() :
    IBuyDecision9ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision9ConfigWidgetFactory";
}

BuyDecision9ConfigWidgetFactory::~BuyDecision9ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision9ConfigWidgetFactory";
}

IBuyDecision9ConfigWidget* BuyDecision9ConfigWidgetFactory::newInstance(IBuyDecision9Config* config, QWidget* parent) const
{
    return new BuyDecision9ConfigWidget(config, parent);
}
