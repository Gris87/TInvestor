#pragma once



#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
#include "src/config/iconfig.h"
#include "src/dialogs/settingsdialog/isettingsdialog.h"
#include "src/storage/user/iuserstorage.h"



struct SettingsDialogFactoryNewInstanceArgsMore15
{
    explicit SettingsDialogFactoryNewInstanceArgsMore15(
        IConfig*                           _config,
        IDecisionMakerConfigWidgetFactory* _decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  _buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  _buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  _buyDecision3ConfigWidgetFactory,
        IBuyDecision4ConfigWidgetFactory*  _buyDecision4ConfigWidgetFactory,
        IBuyDecision5ConfigWidgetFactory*  _buyDecision5ConfigWidgetFactory,
        IBuyDecision6ConfigWidgetFactory*  _buyDecision6ConfigWidgetFactory,
        IBuyDecision7ConfigWidgetFactory*  _buyDecision7ConfigWidgetFactory,
        IBuyDecision8ConfigWidgetFactory*  _buyDecision8ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* _sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* _sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* _sellDecision3ConfigWidgetFactory,
        ISellDecision4ConfigWidgetFactory* _sellDecision4ConfigWidgetFactory,
        IUserStorage*                      _userStorage
    ) :
        config(_config),
        decisionMakerConfigWidgetFactory(_decisionMakerConfigWidgetFactory),
        buyDecision1ConfigWidgetFactory(_buyDecision1ConfigWidgetFactory),
        buyDecision2ConfigWidgetFactory(_buyDecision2ConfigWidgetFactory),
        buyDecision3ConfigWidgetFactory(_buyDecision3ConfigWidgetFactory),
        buyDecision4ConfigWidgetFactory(_buyDecision4ConfigWidgetFactory),
        buyDecision5ConfigWidgetFactory(_buyDecision5ConfigWidgetFactory),
        buyDecision6ConfigWidgetFactory(_buyDecision6ConfigWidgetFactory),
        buyDecision7ConfigWidgetFactory(_buyDecision7ConfigWidgetFactory),
        buyDecision8ConfigWidgetFactory(_buyDecision8ConfigWidgetFactory),
        sellDecision1ConfigWidgetFactory(_sellDecision1ConfigWidgetFactory),
        sellDecision2ConfigWidgetFactory(_sellDecision2ConfigWidgetFactory),
        sellDecision3ConfigWidgetFactory(_sellDecision3ConfigWidgetFactory),
        sellDecision4ConfigWidgetFactory(_sellDecision4ConfigWidgetFactory),
        userStorage(_userStorage)
    {
    }

    bool operator==(const SettingsDialogFactoryNewInstanceArgsMore15& another) const = default;

    IConfig*                           config;
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory;
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory;
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory;
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory;
    IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory;
    IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory;
    IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory;
    IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory;
    IBuyDecision8ConfigWidgetFactory*  buyDecision8ConfigWidgetFactory;
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory;
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory;
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory;
    ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory;
    IUserStorage*                      userStorage;
};



class ISettingsDialogFactory
{
public:
    ISettingsDialogFactory()          = default;
    virtual ~ISettingsDialogFactory() = default;

    ISettingsDialogFactory(const ISettingsDialogFactory& another)            = delete;
    ISettingsDialogFactory& operator=(const ISettingsDialogFactory& another) = delete;

    virtual std::shared_ptr<ISettingsDialog>
    newInstance(const SettingsDialogFactoryNewInstanceArgsMore15& args, QWidget* parent) const = 0;
};
