#include "selldecision3configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/selldecision3configwidget.h"



SellDecision3ConfigWidgetFactory::SellDecision3ConfigWidgetFactory() :
    ISellDecision3ConfigWidgetFactory()
{
    qDebug() << "Create SellDecision3ConfigWidgetFactory";
}

SellDecision3ConfigWidgetFactory::~SellDecision3ConfigWidgetFactory()
{
    qDebug() << "Destroy SellDecision3ConfigWidgetFactory";
}

ISellDecision3ConfigWidget* SellDecision3ConfigWidgetFactory::newInstance(ISellDecision3Config *config, QWidget *parent)
{
    return new SellDecision3ConfigWidget(config, parent);
}
