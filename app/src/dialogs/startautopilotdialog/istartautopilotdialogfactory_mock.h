#pragma once



#include "src/dialogs/startautopilotdialog/istartautopilotdialogfactory.h"

#include <gmock/gmock.h>



class StartAutoPilotDialogFactoryMock : public IStartAutoPilotDialogFactory
{
public:
    StartAutoPilotDialogFactoryMock() :
        IStartAutoPilotDialogFactory()
    {
    }
    ~StartAutoPilotDialogFactoryMock() override = default;

    StartAutoPilotDialogFactoryMock(const StartAutoPilotDialogFactoryMock& another)            = delete;
    StartAutoPilotDialogFactoryMock& operator=(const StartAutoPilotDialogFactoryMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<IStartAutoPilotDialog>, newInstance, (QWidget * parent), (override));
};
