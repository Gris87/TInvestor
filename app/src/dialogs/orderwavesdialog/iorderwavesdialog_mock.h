#pragma once



#include "src/dialogs/orderwavesdialog/iorderwavesdialog.h"

#include <gmock/gmock.h>



class OrderWavesDialogMock : public IOrderWavesDialog
{
public:
    explicit OrderWavesDialogMock(QWidget* parent = nullptr) :
        IOrderWavesDialog(parent)
    {
    }
    ~OrderWavesDialogMock() override = default;

    OrderWavesDialogMock(const OrderWavesDialogMock& another)            = delete;
    OrderWavesDialogMock& operator=(const OrderWavesDialogMock& another) = delete;

    MOCK_METHOD(int, exec, (), (override));
};
