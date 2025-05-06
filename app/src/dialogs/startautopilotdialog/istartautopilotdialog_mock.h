#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialog.h"

#include <gmock/gmock.h>



class StartAutoPilotDialogMock : public IStartAutoPilotDialog
{
    Q_OBJECT

public:
    explicit StartAutoPilotDialogMock(QWidget* parent = nullptr) :
        IStartAutoPilotDialog(parent)
    {
    }
    ~StartAutoPilotDialogMock() override = default;

    StartAutoPilotDialogMock(const StartAutoPilotDialogMock& another)            = delete;
    StartAutoPilotDialogMock& operator=(const StartAutoPilotDialogMock& another) = delete;

    MOCK_METHOD(int, exec, (), (override));
    MOCK_METHOD(QString, account, (), (const, override));
    MOCK_METHOD(QString, mode, (), (const, override));
    MOCK_METHOD(QString, anotherAccount, (), (const, override));
};
