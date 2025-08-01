#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidget.h"



BuyDecision4ConfigWidgetFactory::BuyDecision4ConfigWidgetFactory() :
    IBuyDecision4ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision4ConfigWidgetFactory";
}

BuyDecision4ConfigWidgetFactory::~BuyDecision4ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision4ConfigWidgetFactory";
}

IBuyDecision4ConfigWidget* BuyDecision4ConfigWidgetFactory::newInstance(IBuyDecision4Config* config, QWidget* parent) const
{
    return new BuyDecision4ConfigWidget(config, parent);
}
