#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget_mock.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



using ::testing::StrictMock;
using ::testing::NotNull;
using ::testing::Return;



class Test_DecisionMakerConfigWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        decisionMakerConfigMock              = new StrictMock<DecisionMakerConfigMock>();
        buyDecision1ConfigMock               = new StrictMock<BuyDecision1ConfigMock>();
        buyDecision2ConfigMock               = new StrictMock<BuyDecision2ConfigMock>();
        buyDecision3ConfigMock               = new StrictMock<BuyDecision3ConfigMock>();
        sellDecision1ConfigMock              = new StrictMock<SellDecision1ConfigMock>();
        sellDecision2ConfigMock              = new StrictMock<SellDecision2ConfigMock>();
        sellDecision3ConfigMock              = new StrictMock<SellDecision3ConfigMock>();
        buyDecision1ConfigWidgetMock         = new StrictMock<BuyDecision1ConfigWidgetMock>();
        buyDecision2ConfigWidgetMock         = new StrictMock<BuyDecision2ConfigWidgetMock>();
        buyDecision3ConfigWidgetMock         = new StrictMock<BuyDecision3ConfigWidgetMock>();
        sellDecision1ConfigWidgetMock        = new StrictMock<SellDecision1ConfigWidgetMock>();
        sellDecision2ConfigWidgetMock        = new StrictMock<SellDecision2ConfigWidgetMock>();
        sellDecision3ConfigWidgetMock        = new StrictMock<SellDecision3ConfigWidgetMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision1Config()).WillOnce(Return(buyDecision1ConfigMock));
        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision2Config()).WillOnce(Return(buyDecision2ConfigMock));
        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision3Config()).WillOnce(Return(buyDecision3ConfigMock));
        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision1Config()).WillOnce(Return(sellDecision1ConfigMock));
        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision2Config()).WillOnce(Return(sellDecision2ConfigMock));
        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));

        EXPECT_CALL(*buyDecision1ConfigWidgetFactoryMock, newInstance(buyDecision1ConfigMock, NotNull())).WillOnce(Return(buyDecision1ConfigWidgetMock));
        EXPECT_CALL(*buyDecision2ConfigWidgetFactoryMock, newInstance(buyDecision2ConfigMock, NotNull())).WillOnce(Return(buyDecision2ConfigWidgetMock));
        EXPECT_CALL(*buyDecision3ConfigWidgetFactoryMock, newInstance(buyDecision3ConfigMock, NotNull())).WillOnce(Return(buyDecision3ConfigWidgetMock));
        EXPECT_CALL(*sellDecision1ConfigWidgetFactoryMock, newInstance(sellDecision1ConfigMock, NotNull())).WillOnce(Return(sellDecision1ConfigWidgetMock));
        EXPECT_CALL(*sellDecision2ConfigWidgetFactoryMock, newInstance(sellDecision2ConfigMock, NotNull())).WillOnce(Return(sellDecision2ConfigWidgetMock));
        EXPECT_CALL(*sellDecision3ConfigWidgetFactoryMock, newInstance(sellDecision3ConfigMock, NotNull())).WillOnce(Return(sellDecision3ConfigWidgetMock));

        configWidget = new DecisionMakerConfigWidget(
            decisionMakerConfigMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock
        );
    }

    void TearDown()
    {
        delete configWidget;
        delete decisionMakerConfigMock;
        delete buyDecision1ConfigMock;
        delete buyDecision2ConfigMock;
        delete buyDecision3ConfigMock;
        delete sellDecision1ConfigMock;
        delete sellDecision2ConfigMock;
        delete sellDecision3ConfigMock;
        // It will be deleted by `delete ui;`
        /*
        delete buyDecision1ConfigWidgetMock;
        delete buyDecision2ConfigWidgetMock;
        delete buyDecision3ConfigWidgetMock;
        delete sellDecision1ConfigWidgetMock;
        delete sellDecision2ConfigWidgetMock;
        delete sellDecision3ConfigWidgetMock;
        */
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
    }

    DecisionMakerConfigWidget                        *configWidget;
    StrictMock<DecisionMakerConfigMock>              *decisionMakerConfigMock;
    StrictMock<BuyDecision1ConfigMock>               *buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>               *buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>               *buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock>              *sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock>              *sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock>              *sellDecision3ConfigMock;
    StrictMock<BuyDecision1ConfigWidgetMock>         *buyDecision1ConfigWidgetMock;
    StrictMock<BuyDecision2ConfigWidgetMock>         *buyDecision2ConfigWidgetMock;
    StrictMock<BuyDecision3ConfigWidgetMock>         *buyDecision3ConfigWidgetMock;
    StrictMock<SellDecision1ConfigWidgetMock>        *sellDecision1ConfigWidgetMock;
    StrictMock<SellDecision2ConfigWidgetMock>        *sellDecision2ConfigWidgetMock;
    StrictMock<SellDecision3ConfigWidgetMock>        *sellDecision3ConfigWidgetMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>  *buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>  *buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>  *buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock> *sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock> *sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock> *sellDecision3ConfigWidgetFactoryMock;
};



TEST_F(Test_DecisionMakerConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMakerConfigWidget, Test_updateUiFromConfig)
{
    EXPECT_CALL(*buyDecision1ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision2ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision3ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision1ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision2ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision3ConfigWidgetMock, updateUiFromConfig());

    configWidget->updateUiFromConfig();
}
