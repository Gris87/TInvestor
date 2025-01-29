#include <gtest/gtest.h>

#include "src/config/settingsdialog.h"
#include "src/config/decisions/decisionmakerconfigwidget.h"
#include "src/config/decisions/buy/ibuydecision1config_mock.h"
#include "src/config/decisions/buy/ibuydecision2config_mock.h"
#include "src/config/decisions/buy/ibuydecision3config_mock.h"
#include "src/config/decisions/sell/iselldecision1config_mock.h"
#include "src/config/decisions/sell/iselldecision2config_mock.h"
#include "src/config/decisions/sell/iselldecision3config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/iconfig_mock.h"
#include "ui_buydecision1configwidget.h"
#include "ui_buydecision2configwidget.h"
#include "ui_buydecision3configwidget.h"
#include "ui_decisionmakerconfigwidget.h"
#include "ui_selldecision1configwidget.h"
#include "ui_selldecision2configwidget.h"
#include "ui_selldecision3configwidget.h"



using ::testing::StrictMock;
using ::testing::Return;



class Test_SettingsDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        configMock                       = new StrictMock<ConfigMock>();
        simulatorConfigMock              = new StrictMock<DecisionMakerConfigMock>();
        autoPilotConfigMock              = new StrictMock<DecisionMakerConfigMock>();
        simulatorBuyDecision1ConfigMock  = new StrictMock<BuyDecision1ConfigMock>();
        simulatorBuyDecision2ConfigMock  = new StrictMock<BuyDecision2ConfigMock>();
        simulatorBuyDecision3ConfigMock  = new StrictMock<BuyDecision3ConfigMock>();
        simulatorSellDecision1ConfigMock = new StrictMock<SellDecision1ConfigMock>();
        simulatorSellDecision2ConfigMock = new StrictMock<SellDecision2ConfigMock>();
        simulatorSellDecision3ConfigMock = new StrictMock<SellDecision3ConfigMock>();
        autoPilotBuyDecision1ConfigMock  = new StrictMock<BuyDecision1ConfigMock>();
        autoPilotBuyDecision2ConfigMock  = new StrictMock<BuyDecision2ConfigMock>();
        autoPilotBuyDecision3ConfigMock  = new StrictMock<BuyDecision3ConfigMock>();
        autoPilotSellDecision1ConfigMock = new StrictMock<SellDecision1ConfigMock>();
        autoPilotSellDecision2ConfigMock = new StrictMock<SellDecision2ConfigMock>();
        autoPilotSellDecision3ConfigMock = new StrictMock<SellDecision3ConfigMock>();

        EXPECT_CALL(*configMock, getSimulatorConfig()).WillOnce(Return(simulatorConfigMock));
        EXPECT_CALL(*configMock, getAutoPilotConfig()).WillOnce(Return(autoPilotConfigMock));
        EXPECT_CALL(*simulatorConfigMock, getBuyDecision1Config()).WillOnce(Return(simulatorBuyDecision1ConfigMock));
        EXPECT_CALL(*simulatorConfigMock, getBuyDecision2Config()).WillOnce(Return(simulatorBuyDecision2ConfigMock));
        EXPECT_CALL(*simulatorConfigMock, getBuyDecision3Config()).WillOnce(Return(simulatorBuyDecision3ConfigMock));
        EXPECT_CALL(*simulatorConfigMock, getSellDecision1Config()).WillOnce(Return(simulatorSellDecision1ConfigMock));
        EXPECT_CALL(*simulatorConfigMock, getSellDecision2Config()).WillOnce(Return(simulatorSellDecision2ConfigMock));
        EXPECT_CALL(*simulatorConfigMock, getSellDecision3Config()).WillOnce(Return(simulatorSellDecision3ConfigMock));
        EXPECT_CALL(*autoPilotConfigMock, getBuyDecision1Config()).WillOnce(Return(autoPilotBuyDecision1ConfigMock));
        EXPECT_CALL(*autoPilotConfigMock, getBuyDecision2Config()).WillOnce(Return(autoPilotBuyDecision2ConfigMock));
        EXPECT_CALL(*autoPilotConfigMock, getBuyDecision3Config()).WillOnce(Return(autoPilotBuyDecision3ConfigMock));
        EXPECT_CALL(*autoPilotConfigMock, getSellDecision1Config()).WillOnce(Return(autoPilotSellDecision1ConfigMock));
        EXPECT_CALL(*autoPilotConfigMock, getSellDecision2Config()).WillOnce(Return(autoPilotSellDecision2ConfigMock));
        EXPECT_CALL(*autoPilotConfigMock, getSellDecision3Config()).WillOnce(Return(autoPilotSellDecision3ConfigMock));

        dialog = new SettingsDialog(configMock);
    }

    void TearDown()
    {
        delete dialog;
        delete configMock;
        delete simulatorConfigMock;
        delete autoPilotConfigMock;
        delete simulatorBuyDecision1ConfigMock;
        delete simulatorBuyDecision2ConfigMock;
        delete simulatorBuyDecision3ConfigMock;
        delete simulatorSellDecision1ConfigMock;
        delete simulatorSellDecision2ConfigMock;
        delete simulatorSellDecision3ConfigMock;
        delete autoPilotBuyDecision1ConfigMock;
        delete autoPilotBuyDecision2ConfigMock;
        delete autoPilotBuyDecision3ConfigMock;
        delete autoPilotSellDecision1ConfigMock;
        delete autoPilotSellDecision2ConfigMock;
        delete autoPilotSellDecision3ConfigMock;
    }

    SettingsDialog                      *dialog;
    StrictMock<ConfigMock>              *configMock;
    StrictMock<DecisionMakerConfigMock> *simulatorConfigMock;
    StrictMock<DecisionMakerConfigMock> *autoPilotConfigMock;
    StrictMock<BuyDecision1ConfigMock>  *simulatorBuyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  *simulatorBuyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  *simulatorBuyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> *simulatorSellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> *simulatorSellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> *simulatorSellDecision3ConfigMock;
    StrictMock<BuyDecision1ConfigMock>  *autoPilotBuyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>  *autoPilotBuyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>  *autoPilotBuyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock> *autoPilotSellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock> *autoPilotSellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock> *autoPilotSellDecision3ConfigMock;
};


/*
TEST_F(Test_SettingsDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_SettingsDialog, Test_updateUiFromConfig)
{
}
*/
