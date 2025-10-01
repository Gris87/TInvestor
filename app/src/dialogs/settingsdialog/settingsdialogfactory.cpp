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
    IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
    IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory,
    IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory,
    IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
    IUserStorage*                      userStorage,
    QWidget*                           parent
) const
{
    return std::shared_ptr<ISettingsDialog>(new SettingsDialog(
        config,
        decisionMakerConfigWidgetFactory,
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        buyDecision4ConfigWidgetFactory,
        buyDecision5ConfigWidgetFactory,
        buyDecision6ConfigWidgetFactory,
        buyDecision7ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        sellDecision4ConfigWidgetFactory,
        userStorage,
        parent
    ));
}
