#include "selldecision1configwidgetfactory.h"

#include <QDebug>

#include "src/config/decisions/sell/selldecision1configwidget.h"



SellDecision1ConfigWidgetFactory::SellDecision1ConfigWidgetFactory() :
    ISellDecision1ConfigWidgetFactory()
{
    qDebug() << "Create SellDecision1ConfigWidgetFactory";
}

SellDecision1ConfigWidgetFactory::~SellDecision1ConfigWidgetFactory()
{
    qDebug() << "Destroy SellDecision1ConfigWidgetFactory";
}

ISellDecision1ConfigWidget* SellDecision1ConfigWidgetFactory::newInstance(QWidget *parent)
{
    return new SellDecision1ConfigWidget(parent);
}
