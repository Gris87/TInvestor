#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/selldecision5configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/selldecision5configwidget.h"



SellDecision5ConfigWidgetFactory::SellDecision5ConfigWidgetFactory() :
    ISellDecision5ConfigWidgetFactory()
{
    qDebug() << "Create SellDecision5ConfigWidgetFactory";
}

SellDecision5ConfigWidgetFactory::~SellDecision5ConfigWidgetFactory()
{
    qDebug() << "Destroy SellDecision5ConfigWidgetFactory";
}

ISellDecision5ConfigWidget* SellDecision5ConfigWidgetFactory::newInstance(ISellDecision5Config* config, QWidget* parent) const
{
    return new SellDecision5ConfigWidget(config, parent);
}
