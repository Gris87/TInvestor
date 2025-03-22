#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"

#include <gmock/gmock.h>



class OrderWavesDialogFactoryMock : public IOrderWavesDialogFactory
{
public:
    OrderWavesDialogFactoryMock() :
        IOrderWavesDialogFactory()
    {
    }
    ~OrderWavesDialogFactoryMock() override = default;

    OrderWavesDialogFactoryMock(const OrderWavesDialogFactoryMock& another)            = delete;
    OrderWavesDialogFactoryMock& operator=(const OrderWavesDialogFactoryMock& another) = delete;

    MOCK_METHOD(
        std::shared_ptr<IOrderWavesDialog>,
        newInstance,
        (IOrderWavesWidgetFactory * orderWavesWidgetFactory,
         IOrderBookThread* orderBookThread,
         Stock*            stock,
         int               precision,
         QWidget*          parent),
        (override)
    );
};
