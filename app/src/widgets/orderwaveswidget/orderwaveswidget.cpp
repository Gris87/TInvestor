#include "src/widgets/orderwaveswidget/orderwaveswidget.h"

#include <QDebug>



OrderWavesWidget::OrderWavesWidget(QWidget* parent) :
    IOrderWavesWidget(parent)
{
    qDebug() << "Create OrderWavesWidget";
}

OrderWavesWidget::~OrderWavesWidget()
{
    qDebug() << "Destroy OrderWavesWidget";
}
