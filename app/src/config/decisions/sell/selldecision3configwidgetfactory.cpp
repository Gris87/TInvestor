#include "selldecision3configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/sell/selldecision3configwidget.h"



SellDecision3ConfigWidgetFactory::SellDecision3ConfigWidgetFactory() :
    ISellDecision3ConfigWidgetFactory()
{
    qDebug() << "Create SellDecision3ConfigWidgetFactory";
}

SellDecision3ConfigWidgetFactory::~SellDecision3ConfigWidgetFactory()
{
    qDebug() << "Destroy SellDecision3ConfigWidgetFactory";
}

ISellDecision3ConfigWidget* SellDecision3ConfigWidgetFactory::newInstance(QWidget *parent)
{
    return new SellDecision3ConfigWidget(parent);
}
