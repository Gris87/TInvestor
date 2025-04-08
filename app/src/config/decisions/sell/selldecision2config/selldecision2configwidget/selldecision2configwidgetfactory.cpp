#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidget.h"



SellDecision2ConfigWidgetFactory::SellDecision2ConfigWidgetFactory() :
    ISellDecision2ConfigWidgetFactory()
{
    qDebug() << "Create SellDecision2ConfigWidgetFactory";
}

SellDecision2ConfigWidgetFactory::~SellDecision2ConfigWidgetFactory()
{
    qDebug() << "Destroy SellDecision2ConfigWidgetFactory";
}

ISellDecision2ConfigWidget* SellDecision2ConfigWidgetFactory::newInstance(ISellDecision2Config* config, QWidget* parent) const
{
    return new SellDecision2ConfigWidget(config, parent);
}
