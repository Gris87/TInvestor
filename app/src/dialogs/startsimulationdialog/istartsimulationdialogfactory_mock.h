#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialogfactory.h"

#include <gmock/gmock.h>



class StartSimulationDialogFactoryMock : public IStartSimulationDialogFactory
{
public:
    StartSimulationDialogFactoryMock() :
        IStartSimulationDialogFactory()
    {
    }
    ~StartSimulationDialogFactoryMock() override = default;

    StartSimulationDialogFactoryMock(const StartSimulationDialogFactoryMock& another)            = delete;
    StartSimulationDialogFactoryMock& operator=(const StartSimulationDialogFactoryMock& another) = delete;

    MOCK_METHOD(
        std::shared_ptr<IStartSimulationDialog>,
        newInstance,
        (ISettingsEditor * settingsEditor, QWidget* parent),
        (const, override)
    );
};
