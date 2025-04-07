#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialog.h"

#include <gmock/gmock.h>



class StartSimulationDialogMock : public IStartSimulationDialog
{
    Q_OBJECT

public:
    explicit StartSimulationDialogMock(QWidget* parent = nullptr) :
        IStartSimulationDialog(parent)
    {
    }
    ~StartSimulationDialogMock() override = default;

    StartSimulationDialogMock(const StartSimulationDialogMock& another)            = delete;
    StartSimulationDialogMock& operator=(const StartSimulationDialogMock& another) = delete;

    MOCK_METHOD(int, exec, (), (override));
};
