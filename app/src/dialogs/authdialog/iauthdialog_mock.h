#pragma once



#include "src/dialogs/authdialog/iauthdialog.h"

#include <gmock/gmock.h>



class AuthDialogMock : public IAuthDialog
{
    Q_OBJECT

public:
    explicit AuthDialogMock(QWidget* parent = nullptr) :
        IAuthDialog(parent)
    {
    }
    ~AuthDialogMock() override = default;

    AuthDialogMock(const AuthDialogMock& another)            = delete;
    AuthDialogMock& operator=(const AuthDialogMock& another) = delete;

    MOCK_METHOD(QString, getToken, (), (override));
    MOCK_METHOD(int, exec, (), (override));
};
