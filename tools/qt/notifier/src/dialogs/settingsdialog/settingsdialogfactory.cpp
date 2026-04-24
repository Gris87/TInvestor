#include "src/dialogs/settingsdialog/settingsdialogfactory.h"

#include <QDebug>

#include "src/dialogs/settingsdialog/settingsdialog.h"



SettingsDialogFactory::SettingsDialogFactory() :
    ISettingsDialogFactory()
{
    qDebug() << "Create SettingsDialogFactory";
}

SettingsDialogFactory::~SettingsDialogFactory()
{
    qDebug() << "Destroy SettingsDialogFactory";
}

std::shared_ptr<ISettingsDialog> SettingsDialogFactory::newInstance(IConfig* config, QWidget* parent) const
{
    return std::shared_ptr<ISettingsDialog>(new SettingsDialog(config, parent));
}
