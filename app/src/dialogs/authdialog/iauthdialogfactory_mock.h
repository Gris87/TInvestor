#pragma once



#include "src/dialogs/authdialog/iauthdialogfactory.h"

#include <gmock/gmock.h>



class AuthDialogFactoryMock : public IAuthDialogFactory
{
public:
    AuthDialogFactoryMock() :
        IAuthDialogFactory()
    {
    }
    ~AuthDialogFactoryMock() override = default;

    AuthDialogFactoryMock(const AuthDialogFactoryMock& another)            = delete;
    AuthDialogFactoryMock& operator=(const AuthDialogFactoryMock& another) = delete;

    MOCK_METHOD(IAuthDialog*, newInstance, (QWidget * parent), (override));
};
