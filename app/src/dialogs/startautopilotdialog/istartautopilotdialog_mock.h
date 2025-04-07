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
};
