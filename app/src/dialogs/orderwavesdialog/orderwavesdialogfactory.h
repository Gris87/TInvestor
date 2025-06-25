#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"



class OrderWavesDialogFactory : public IOrderWavesDialogFactory
{
public:
    OrderWavesDialogFactory();
    ~OrderWavesDialogFactory() override;

    OrderWavesDialogFactory(const OrderWavesDialogFactory& another)            = delete;
    OrderWavesDialogFactory& operator=(const OrderWavesDialogFactory& another) = delete;

    std::shared_ptr<IOrderWavesDialog> newInstance(
        IOrderWavesWidgetFactory* orderWavesWidgetFactory, IOrderBookThread* orderBookThread, Stock* stock, QWidget* parent
    ) const override;
};
