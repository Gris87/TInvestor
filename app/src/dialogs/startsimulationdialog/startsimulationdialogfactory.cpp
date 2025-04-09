#include "src/dialogs/startsimulationdialog/startsimulationdialogfactory.h"

#include <QDebug>

#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"



StartSimulationDialogFactory::StartSimulationDialogFactory() :
    IStartSimulationDialogFactory()
{
    qDebug() << "Create StartSimulationDialogFactory";
}

StartSimulationDialogFactory::~StartSimulationDialogFactory()
{
    qDebug() << "Destroy StartSimulationDialogFactory";
}

std::shared_ptr<IStartSimulationDialog>
StartSimulationDialogFactory::newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const
{
    return std::shared_ptr<IStartSimulationDialog>(new StartSimulationDialog(settingsEditor, parent));
}
