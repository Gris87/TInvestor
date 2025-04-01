#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"

#include <QDebug>

#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"



StartAutoPilotDialogFactory::StartAutoPilotDialogFactory() :
    IStartAutoPilotDialogFactory()
{
    qDebug() << "Create StartAutoPilotDialogFactory";
}

StartAutoPilotDialogFactory::~StartAutoPilotDialogFactory()
{
    qDebug() << "Destroy StartAutoPilotDialogFactory";
}

std::shared_ptr<IStartAutoPilotDialog> StartAutoPilotDialogFactory::newInstance(ISettingsEditor* settingsEditor, QWidget* parent)
{
    return std::shared_ptr<IStartAutoPilotDialog>(new StartAutoPilotDialog(settingsEditor, parent));
}
