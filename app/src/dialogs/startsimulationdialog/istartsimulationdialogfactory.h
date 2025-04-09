#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialog.h"

#include "src/utils/settingseditor/isettingseditor.h"



class IStartSimulationDialogFactory
{
public:
    IStartSimulationDialogFactory()          = default;
    virtual ~IStartSimulationDialogFactory() = default;

    IStartSimulationDialogFactory(const IStartSimulationDialogFactory& another)            = delete;
    IStartSimulationDialogFactory& operator=(const IStartSimulationDialogFactory& another) = delete;

    virtual std::shared_ptr<IStartSimulationDialog> newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const = 0;
};
