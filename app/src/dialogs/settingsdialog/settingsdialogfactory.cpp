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

std::shared_ptr<ISettingsDialog>
SettingsDialogFactory::newInstance(const SettingsDialogFactoryNewInstanceArgsMore15& args, QWidget* parent) const
{
    return std::shared_ptr<ISettingsDialog>(new SettingsDialog(
        args.config,
        args.decisionMakerConfigWidgetFactory,
        args.buyDecision1ConfigWidgetFactory,
        args.buyDecision2ConfigWidgetFactory,
        args.buyDecision3ConfigWidgetFactory,
        args.buyDecision4ConfigWidgetFactory,
        args.buyDecision5ConfigWidgetFactory,
        args.buyDecision6ConfigWidgetFactory,
        args.buyDecision7ConfigWidgetFactory,
        args.buyDecision8ConfigWidgetFactory,
        args.sellDecision1ConfigWidgetFactory,
        args.sellDecision2ConfigWidgetFactory,
        args.sellDecision3ConfigWidgetFactory,
        args.sellDecision4ConfigWidgetFactory,
        args.sellDecision5ConfigWidgetFactory,
        args.userStorage,
        parent
    ));
}
