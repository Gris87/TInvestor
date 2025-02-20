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

std::shared_ptr<ISettingsDialog> SettingsDialogFactory::newInstance(
    IConfig*                           config,
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    QWidget*                           parent
)
{
    return std::shared_ptr<ISettingsDialog>(new SettingsDialog(
        config,
        decisionMakerConfigWidgetFactory,
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        parent
    ));
}
