#include "src/widgets/orderwaveswidget/orderwaveswidgetfactory.h"

#include <QDebug>

#include "src/widgets/orderwaveswidget/orderwaveswidget.h"



OrderWavesWidgetFactory::OrderWavesWidgetFactory() :
    IOrderWavesWidgetFactory()
{
    qDebug() << "Create OrderWavesWidgetFactory";
}

OrderWavesWidgetFactory::~OrderWavesWidgetFactory()
{
    qDebug() << "Destroy OrderWavesWidgetFactory";
}

IOrderWavesWidget* OrderWavesWidgetFactory::newInstance(QWidget* parent)
{
    return new OrderWavesWidget(parent);
}
