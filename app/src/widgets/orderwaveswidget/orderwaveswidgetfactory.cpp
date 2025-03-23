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

IOrderWavesWidget* OrderWavesWidgetFactory::newInstance(int precision, float priceIncrement, QWidget* parent)
{
    return new OrderWavesWidget(precision, priceIncrement, parent);
}
