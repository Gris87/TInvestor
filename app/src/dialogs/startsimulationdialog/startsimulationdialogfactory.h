#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialogfactory.h"



class StartSimulationDialogFactory : public IStartSimulationDialogFactory
{
public:
    StartSimulationDialogFactory();
    ~StartSimulationDialogFactory();

    StartSimulationDialogFactory(const StartSimulationDialogFactory& another)            = delete;
    StartSimulationDialogFactory& operator=(const StartSimulationDialogFactory& another) = delete;

    std::shared_ptr<IStartSimulationDialog> newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const override;
};
