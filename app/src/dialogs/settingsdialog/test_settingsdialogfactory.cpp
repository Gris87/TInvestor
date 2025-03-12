#include "src/dialogs/settingsdialog/settingsdialogfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidget_mock.h"
#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/iconfig_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_SettingsDialogFactory, Test_constructor_and_destructor)
{
    SettingsDialogFactory factory;
}

TEST(Test_SettingsDialogFactory, Test_newInstance)
{
    InSequence seq;

    SettingsDialogFactory factory;

    StrictMock<ConfigMock>                           configMock;
    StrictMock<DecisionMakerConfigMock>              simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock>              autoPilotConfigMock;
    StrictMock<DecisionMakerConfigWidgetMock>*       simulatorConfigWidgetMock = new StrictMock<DecisionMakerConfigWidgetMock>();
    StrictMock<DecisionMakerConfigWidgetMock>*       autoPilotConfigWidgetMock = new StrictMock<DecisionMakerConfigWidgetMock>();
    StrictMock<DecisionMakerConfigWidgetFactoryMock> decisionMakerConfigWidgetFactoryMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock> sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock> sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock> sellDecision3ConfigWidgetFactoryMock;

    EXPECT_CALL(configMock, getSimulatorConfig()).WillOnce(Return(&simulatorConfigMock));
    EXPECT_CALL(
        decisionMakerConfigWidgetFactoryMock,
        newInstance(
            &simulatorConfigMock,
            &buyDecision1ConfigWidgetFactoryMock,
            &buyDecision2ConfigWidgetFactoryMock,
            &buyDecision3ConfigWidgetFactoryMock,
            &sellDecision1ConfigWidgetFactoryMock,
            &sellDecision2ConfigWidgetFactoryMock,
            &sellDecision3ConfigWidgetFactoryMock,
            NotNull()
        )
    )
        .WillOnce(Return(simulatorConfigWidgetMock));

    EXPECT_CALL(configMock, getAutoPilotConfig()).WillOnce(Return(&autoPilotConfigMock));
    EXPECT_CALL(
        decisionMakerConfigWidgetFactoryMock,
        newInstance(
            &autoPilotConfigMock,
            &buyDecision1ConfigWidgetFactoryMock,
            &buyDecision2ConfigWidgetFactoryMock,
            &buyDecision3ConfigWidgetFactoryMock,
            &sellDecision1ConfigWidgetFactoryMock,
            &sellDecision2ConfigWidgetFactoryMock,
            &sellDecision3ConfigWidgetFactoryMock,
            NotNull()
        )
    )
        .WillOnce(Return(autoPilotConfigWidgetMock));

    std::shared_ptr<ISettingsDialog> dialog = factory.newInstance(
        &configMock,
        &decisionMakerConfigWidgetFactoryMock,
        &buyDecision1ConfigWidgetFactoryMock,
        &buyDecision2ConfigWidgetFactoryMock,
        &buyDecision3ConfigWidgetFactoryMock,
        &sellDecision1ConfigWidgetFactoryMock,
        &sellDecision2ConfigWidgetFactoryMock,
        &sellDecision3ConfigWidgetFactoryMock,
        nullptr
    );

    ASSERT_TRUE(dialog != nullptr);
}
