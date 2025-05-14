#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"

#include <QDebug>

#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"



OrderWavesDialogFactory::OrderWavesDialogFactory() :
    IOrderWavesDialogFactory()
{
    qDebug() << "Create OrderWavesDialogFactory";
}

OrderWavesDialogFactory::~OrderWavesDialogFactory()
{
    qDebug() << "Destroy OrderWavesDialogFactory";
}

std::shared_ptr<IOrderWavesDialog> OrderWavesDialogFactory::newInstance(
    IOrderWavesWidgetFactory* orderWavesWidgetFactory,
    IOrderBookThread*         orderBookThread,
    Stock*                    stock,
    qint8                     precision,
    QWidget*                  parent
) const
{
    return std::shared_ptr<IOrderWavesDialog>(
        new OrderWavesDialog(orderWavesWidgetFactory, orderBookThread, stock, precision, parent)
    );
}
