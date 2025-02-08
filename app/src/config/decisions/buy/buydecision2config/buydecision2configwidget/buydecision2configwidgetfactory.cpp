#include "buydecision2configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidget.h"



BuyDecision2ConfigWidgetFactory::BuyDecision2ConfigWidgetFactory() :
    IBuyDecision2ConfigWidgetFactory()
{
    qDebug() << "Create BuyDecision2ConfigWidgetFactory";
}

BuyDecision2ConfigWidgetFactory::~BuyDecision2ConfigWidgetFactory()
{
    qDebug() << "Destroy BuyDecision2ConfigWidgetFactory";
}

IBuyDecision2ConfigWidget* BuyDecision2ConfigWidgetFactory::newInstance(IBuyDecision2Config* config, QWidget* parent)
{
    return new BuyDecision2ConfigWidget(config, parent);
}
