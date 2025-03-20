#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"



class OrderWavesDialogFactory : public IOrderWavesDialogFactory
{
public:
    OrderWavesDialogFactory();
    ~OrderWavesDialogFactory();

    OrderWavesDialogFactory(const OrderWavesDialogFactory& another)            = delete;
    OrderWavesDialogFactory& operator=(const OrderWavesDialogFactory& another) = delete;

    std::shared_ptr<IOrderWavesDialog>
    newInstance(IOrderBookThread* orderBookThread, Stock* stock, QWidget* parent) override;
};
