#pragma once



#include "src/dialogs/startsimulationdialog/istartsimulationdialog.h"



class IStartSimulationDialogFactory
{
public:
    IStartSimulationDialogFactory()
    {
    }
    virtual ~IStartSimulationDialogFactory() = default;

    IStartSimulationDialogFactory(const IStartSimulationDialogFactory& another)            = delete;
    IStartSimulationDialogFactory& operator=(const IStartSimulationDialogFactory& another) = delete;

    virtual std::shared_ptr<IStartSimulationDialog> newInstance(QWidget* parent) = 0;
};
