#pragma once



#include "src/config/settingsdialog/isettingsdialogfactory.h"

#include <gmock/gmock.h>



class SettingsDialogFactoryMock : public ISettingsDialogFactory
{
public:
    SettingsDialogFactoryMock() : ISettingsDialogFactory() {}
    ~SettingsDialogFactoryMock() override = default;

    SettingsDialogFactoryMock(const SettingsDialogFactoryMock &another) = delete;
    SettingsDialogFactoryMock& operator=(const SettingsDialogFactoryMock &another) = delete;

    MOCK_METHOD9(newInstance, ISettingsDialog*(
        IConfig *config,
        IDecisionMakerConfigWidgetFactory *decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent
    ));
};
