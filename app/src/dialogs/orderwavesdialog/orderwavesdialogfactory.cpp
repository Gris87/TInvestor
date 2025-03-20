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

std::shared_ptr<IOrderWavesDialog>
OrderWavesDialogFactory::newInstance(IOrderBookThread* orderBookThread, Stock* stock, QWidget* parent)
{
    return std::shared_ptr<IOrderWavesDialog>(new OrderWavesDialog(orderBookThread, stock, parent));
}
