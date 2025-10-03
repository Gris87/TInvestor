#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidget.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget_mock.h"
#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget_mock.h"
#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget_mock.h"
#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidget_mock.h"
#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"
#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidget_mock.h"
#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"
#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidget_mock.h"
#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision6config/ibuydecision6config_mock.h"
#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidget_mock.h"
#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision7config/ibuydecision7config_mock.h"
#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidget_mock.h"
#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidgetfactory_mock.h"
#include "src/config/decisions/buy/buydecision8config/ibuydecision8config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget_mock.h"
#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidget_mock.h"
#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/iselldecision2configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidget_mock.h"
#include "src/config/decisions/sell/selldecision3config/selldecision3configwidget/iselldecision3configwidgetfactory_mock.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidget_mock.h"
#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_DecisionMakerConfigWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        decisionMakerConfigMock              = new StrictMock<DecisionMakerConfigMock>();
        buyDecision1ConfigMock               = new StrictMock<BuyDecision1ConfigMock>();
        buyDecision2ConfigMock               = new StrictMock<BuyDecision2ConfigMock>();
        buyDecision3ConfigMock               = new StrictMock<BuyDecision3ConfigMock>();
        buyDecision4ConfigMock               = new StrictMock<BuyDecision4ConfigMock>();
        buyDecision5ConfigMock               = new StrictMock<BuyDecision5ConfigMock>();
        buyDecision6ConfigMock               = new StrictMock<BuyDecision6ConfigMock>();
        buyDecision7ConfigMock               = new StrictMock<BuyDecision7ConfigMock>();
        buyDecision8ConfigMock               = new StrictMock<BuyDecision8ConfigMock>();
        sellDecision1ConfigMock              = new StrictMock<SellDecision1ConfigMock>();
        sellDecision2ConfigMock              = new StrictMock<SellDecision2ConfigMock>();
        sellDecision3ConfigMock              = new StrictMock<SellDecision3ConfigMock>();
        sellDecision4ConfigMock              = new StrictMock<SellDecision4ConfigMock>();
        buyDecision1ConfigWidgetMock         = new StrictMock<BuyDecision1ConfigWidgetMock>();
        buyDecision2ConfigWidgetMock         = new StrictMock<BuyDecision2ConfigWidgetMock>();
        buyDecision3ConfigWidgetMock         = new StrictMock<BuyDecision3ConfigWidgetMock>();
        buyDecision4ConfigWidgetMock         = new StrictMock<BuyDecision4ConfigWidgetMock>();
        buyDecision5ConfigWidgetMock         = new StrictMock<BuyDecision5ConfigWidgetMock>();
        buyDecision6ConfigWidgetMock         = new StrictMock<BuyDecision6ConfigWidgetMock>();
        buyDecision7ConfigWidgetMock         = new StrictMock<BuyDecision7ConfigWidgetMock>();
        buyDecision8ConfigWidgetMock         = new StrictMock<BuyDecision8ConfigWidgetMock>();
        sellDecision1ConfigWidgetMock        = new StrictMock<SellDecision1ConfigWidgetMock>();
        sellDecision2ConfigWidgetMock        = new StrictMock<SellDecision2ConfigWidgetMock>();
        sellDecision3ConfigWidgetMock        = new StrictMock<SellDecision3ConfigWidgetMock>();
        sellDecision4ConfigWidgetMock        = new StrictMock<SellDecision4ConfigWidgetMock>();
        buyDecision1ConfigWidgetFactoryMock  = new StrictMock<BuyDecision1ConfigWidgetFactoryMock>();
        buyDecision2ConfigWidgetFactoryMock  = new StrictMock<BuyDecision2ConfigWidgetFactoryMock>();
        buyDecision3ConfigWidgetFactoryMock  = new StrictMock<BuyDecision3ConfigWidgetFactoryMock>();
        buyDecision4ConfigWidgetFactoryMock  = new StrictMock<BuyDecision4ConfigWidgetFactoryMock>();
        buyDecision5ConfigWidgetFactoryMock  = new StrictMock<BuyDecision5ConfigWidgetFactoryMock>();
        buyDecision6ConfigWidgetFactoryMock  = new StrictMock<BuyDecision6ConfigWidgetFactoryMock>();
        buyDecision7ConfigWidgetFactoryMock  = new StrictMock<BuyDecision7ConfigWidgetFactoryMock>();
        buyDecision8ConfigWidgetFactoryMock  = new StrictMock<BuyDecision8ConfigWidgetFactoryMock>();
        sellDecision1ConfigWidgetFactoryMock = new StrictMock<SellDecision1ConfigWidgetFactoryMock>();
        sellDecision2ConfigWidgetFactoryMock = new StrictMock<SellDecision2ConfigWidgetFactoryMock>();
        sellDecision3ConfigWidgetFactoryMock = new StrictMock<SellDecision3ConfigWidgetFactoryMock>();
        sellDecision4ConfigWidgetFactoryMock = new StrictMock<SellDecision4ConfigWidgetFactoryMock>();

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision1Config()).WillOnce(Return(buyDecision1ConfigMock));
        EXPECT_CALL(*buyDecision1ConfigWidgetFactoryMock, newInstance(buyDecision1ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision1ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision2Config()).WillOnce(Return(buyDecision2ConfigMock));
        EXPECT_CALL(*buyDecision2ConfigWidgetFactoryMock, newInstance(buyDecision2ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision2ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision3Config()).WillOnce(Return(buyDecision3ConfigMock));
        EXPECT_CALL(*buyDecision3ConfigWidgetFactoryMock, newInstance(buyDecision3ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision3ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision4Config()).WillOnce(Return(buyDecision4ConfigMock));
        EXPECT_CALL(*buyDecision4ConfigWidgetFactoryMock, newInstance(buyDecision4ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision4ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision5Config()).WillOnce(Return(buyDecision5ConfigMock));
        EXPECT_CALL(*buyDecision5ConfigWidgetFactoryMock, newInstance(buyDecision5ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision5ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision6Config()).WillOnce(Return(buyDecision6ConfigMock));
        EXPECT_CALL(*buyDecision6ConfigWidgetFactoryMock, newInstance(buyDecision6ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision6ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision7Config()).WillOnce(Return(buyDecision7ConfigMock));
        EXPECT_CALL(*buyDecision7ConfigWidgetFactoryMock, newInstance(buyDecision7ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision7ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getBuyDecision8Config()).WillOnce(Return(buyDecision8ConfigMock));
        EXPECT_CALL(*buyDecision8ConfigWidgetFactoryMock, newInstance(buyDecision8ConfigMock, NotNull()))
            .WillOnce(Return(buyDecision8ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision1Config()).WillOnce(Return(sellDecision1ConfigMock));
        EXPECT_CALL(*sellDecision1ConfigWidgetFactoryMock, newInstance(sellDecision1ConfigMock, NotNull()))
            .WillOnce(Return(sellDecision1ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision2Config()).WillOnce(Return(sellDecision2ConfigMock));
        EXPECT_CALL(*sellDecision2ConfigWidgetFactoryMock, newInstance(sellDecision2ConfigMock, NotNull()))
            .WillOnce(Return(sellDecision2ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision3Config()).WillOnce(Return(sellDecision3ConfigMock));
        EXPECT_CALL(*sellDecision3ConfigWidgetFactoryMock, newInstance(sellDecision3ConfigMock, NotNull()))
            .WillOnce(Return(sellDecision3ConfigWidgetMock));

        EXPECT_CALL(*decisionMakerConfigMock, getSellDecision4Config()).WillOnce(Return(sellDecision4ConfigMock));
        EXPECT_CALL(*sellDecision4ConfigWidgetFactoryMock, newInstance(sellDecision4ConfigMock, NotNull()))
            .WillOnce(Return(sellDecision4ConfigWidgetMock));

        configWidget = new DecisionMakerConfigWidget(
            decisionMakerConfigMock,
            buyDecision1ConfigWidgetFactoryMock,
            buyDecision2ConfigWidgetFactoryMock,
            buyDecision3ConfigWidgetFactoryMock,
            buyDecision4ConfigWidgetFactoryMock,
            buyDecision5ConfigWidgetFactoryMock,
            buyDecision6ConfigWidgetFactoryMock,
            buyDecision7ConfigWidgetFactoryMock,
            buyDecision8ConfigWidgetFactoryMock,
            sellDecision1ConfigWidgetFactoryMock,
            sellDecision2ConfigWidgetFactoryMock,
            sellDecision3ConfigWidgetFactoryMock,
            sellDecision4ConfigWidgetFactoryMock
        );
    }

    void TearDown() override
    {
        delete configWidget;
        delete decisionMakerConfigMock;
        delete buyDecision1ConfigMock;
        delete buyDecision2ConfigMock;
        delete buyDecision3ConfigMock;
        delete buyDecision4ConfigMock;
        delete buyDecision5ConfigMock;
        delete buyDecision6ConfigMock;
        delete buyDecision7ConfigMock;
        delete buyDecision8ConfigMock;
        delete sellDecision1ConfigMock;
        delete sellDecision2ConfigMock;
        delete sellDecision3ConfigMock;
        delete sellDecision4ConfigMock;
        // It will be deleted by `delete ui;`
        /*
        delete buyDecision1ConfigWidgetMock;
        delete buyDecision2ConfigWidgetMock;
        delete buyDecision3ConfigWidgetMock;
        delete buyDecision4ConfigWidgetMock;
        delete buyDecision5ConfigWidgetMock;
        delete buyDecision6ConfigWidgetMock;
        delete buyDecision7ConfigWidgetMock;
        delete buyDecision8ConfigWidgetMock;
        delete sellDecision1ConfigWidgetMock;
        delete sellDecision2ConfigWidgetMock;
        delete sellDecision3ConfigWidgetMock;
        delete sellDecision4ConfigWidgetMock;
        */
        delete buyDecision1ConfigWidgetFactoryMock;
        delete buyDecision2ConfigWidgetFactoryMock;
        delete buyDecision3ConfigWidgetFactoryMock;
        delete buyDecision4ConfigWidgetFactoryMock;
        delete buyDecision5ConfigWidgetFactoryMock;
        delete buyDecision6ConfigWidgetFactoryMock;
        delete buyDecision7ConfigWidgetFactoryMock;
        delete buyDecision8ConfigWidgetFactoryMock;
        delete sellDecision1ConfigWidgetFactoryMock;
        delete sellDecision2ConfigWidgetFactoryMock;
        delete sellDecision3ConfigWidgetFactoryMock;
        delete sellDecision4ConfigWidgetFactoryMock;
    }

    DecisionMakerConfigWidget*                        configWidget;
    StrictMock<DecisionMakerConfigMock>*              decisionMakerConfigMock;
    StrictMock<BuyDecision1ConfigMock>*               buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>*               buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>*               buyDecision3ConfigMock;
    StrictMock<BuyDecision4ConfigMock>*               buyDecision4ConfigMock;
    StrictMock<BuyDecision5ConfigMock>*               buyDecision5ConfigMock;
    StrictMock<BuyDecision6ConfigMock>*               buyDecision6ConfigMock;
    StrictMock<BuyDecision7ConfigMock>*               buyDecision7ConfigMock;
    StrictMock<BuyDecision8ConfigMock>*               buyDecision8ConfigMock;
    StrictMock<SellDecision1ConfigMock>*              sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock>*              sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock>*              sellDecision3ConfigMock;
    StrictMock<SellDecision4ConfigMock>*              sellDecision4ConfigMock;
    StrictMock<BuyDecision1ConfigWidgetMock>*         buyDecision1ConfigWidgetMock;
    StrictMock<BuyDecision2ConfigWidgetMock>*         buyDecision2ConfigWidgetMock;
    StrictMock<BuyDecision3ConfigWidgetMock>*         buyDecision3ConfigWidgetMock;
    StrictMock<BuyDecision4ConfigWidgetMock>*         buyDecision4ConfigWidgetMock;
    StrictMock<BuyDecision5ConfigWidgetMock>*         buyDecision5ConfigWidgetMock;
    StrictMock<BuyDecision6ConfigWidgetMock>*         buyDecision6ConfigWidgetMock;
    StrictMock<BuyDecision7ConfigWidgetMock>*         buyDecision7ConfigWidgetMock;
    StrictMock<BuyDecision8ConfigWidgetMock>*         buyDecision8ConfigWidgetMock;
    StrictMock<SellDecision1ConfigWidgetMock>*        sellDecision1ConfigWidgetMock;
    StrictMock<SellDecision2ConfigWidgetMock>*        sellDecision2ConfigWidgetMock;
    StrictMock<SellDecision3ConfigWidgetMock>*        sellDecision3ConfigWidgetMock;
    StrictMock<SellDecision4ConfigWidgetMock>*        sellDecision4ConfigWidgetMock;
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>*  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>*  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>*  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<BuyDecision4ConfigWidgetFactoryMock>*  buyDecision4ConfigWidgetFactoryMock;
    StrictMock<BuyDecision5ConfigWidgetFactoryMock>*  buyDecision5ConfigWidgetFactoryMock;
    StrictMock<BuyDecision6ConfigWidgetFactoryMock>*  buyDecision6ConfigWidgetFactoryMock;
    StrictMock<BuyDecision7ConfigWidgetFactoryMock>*  buyDecision7ConfigWidgetFactoryMock;
    StrictMock<BuyDecision8ConfigWidgetFactoryMock>*  buyDecision8ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock>* sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock>* sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock>* sellDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision4ConfigWidgetFactoryMock>* sellDecision4ConfigWidgetFactoryMock;
};



TEST_F(Test_DecisionMakerConfigWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_DecisionMakerConfigWidget, Test_updateUiFromConfig)
{
    const InSequence seq;

    EXPECT_CALL(*buyDecision1ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision2ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision3ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision4ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision5ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision6ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision7ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*buyDecision8ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision1ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision2ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision3ConfigWidgetMock, updateUiFromConfig());
    EXPECT_CALL(*sellDecision4ConfigWidgetMock, updateUiFromConfig());

    configWidget->updateUiFromConfig();
}

TEST_F(Test_DecisionMakerConfigWidget, Test_makeReadOnly)
{
    const InSequence seq;

    EXPECT_CALL(*buyDecision1ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision2ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision3ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision4ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision5ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision6ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision7ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*buyDecision8ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*sellDecision1ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*sellDecision2ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*sellDecision3ConfigWidgetMock, makeReadOnly());
    EXPECT_CALL(*sellDecision4ConfigWidgetMock, makeReadOnly());

    configWidget->makeReadOnly();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
