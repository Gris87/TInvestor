#include "src/config/decisions/decisionmakerconfigwidget/decisionmakerconfigwidgetfactory.h"

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



TEST(Test_DecisionMakerConfigWidgetFactory, Test_constructor_and_destructor)
{
    DecisionMakerConfigWidgetFactory factory;
}

TEST(Test_DecisionMakerConfigWidgetFactory, Test_newInstance)
{
    DecisionMakerConfigWidgetFactory factory;

    StrictMock<DecisionMakerConfigMock>              decisionMakerConfigMock;
    StrictMock<BuyDecision1ConfigMock>               buyDecision1ConfigMock;
    StrictMock<BuyDecision2ConfigMock>               buyDecision2ConfigMock;
    StrictMock<BuyDecision3ConfigMock>               buyDecision3ConfigMock;
    StrictMock<SellDecision1ConfigMock>              sellDecision1ConfigMock;
    StrictMock<SellDecision2ConfigMock>              sellDecision2ConfigMock;
    StrictMock<SellDecision3ConfigMock>              sellDecision3ConfigMock;
    StrictMock<BuyDecision1ConfigWidgetMock>         *buyDecision1ConfigWidgetMock  = new StrictMock<BuyDecision1ConfigWidgetMock>();
    StrictMock<BuyDecision2ConfigWidgetMock>         *buyDecision2ConfigWidgetMock  = new StrictMock<BuyDecision2ConfigWidgetMock>();
    StrictMock<BuyDecision3ConfigWidgetMock>         *buyDecision3ConfigWidgetMock  = new StrictMock<BuyDecision3ConfigWidgetMock>();
    StrictMock<SellDecision1ConfigWidgetMock>        *sellDecision1ConfigWidgetMock = new StrictMock<SellDecision1ConfigWidgetMock>();
    StrictMock<SellDecision2ConfigWidgetMock>        *sellDecision2ConfigWidgetMock = new StrictMock<SellDecision2ConfigWidgetMock>();
    StrictMock<SellDecision3ConfigWidgetMock>        *sellDecision3ConfigWidgetMock = new StrictMock<SellDecision3ConfigWidgetMock>();
    StrictMock<BuyDecision1ConfigWidgetFactoryMock>  buyDecision1ConfigWidgetFactoryMock;
    StrictMock<BuyDecision2ConfigWidgetFactoryMock>  buyDecision2ConfigWidgetFactoryMock;
    StrictMock<BuyDecision3ConfigWidgetFactoryMock>  buyDecision3ConfigWidgetFactoryMock;
    StrictMock<SellDecision1ConfigWidgetFactoryMock> sellDecision1ConfigWidgetFactoryMock;
    StrictMock<SellDecision2ConfigWidgetFactoryMock> sellDecision2ConfigWidgetFactoryMock;
    StrictMock<SellDecision3ConfigWidgetFactoryMock> sellDecision3ConfigWidgetFactoryMock;

    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision1Config()).WillOnce(Return(&buyDecision1ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision2Config()).WillOnce(Return(&buyDecision2ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getBuyDecision3Config()).WillOnce(Return(&buyDecision3ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision1Config()).WillOnce(Return(&sellDecision1ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision2Config()).WillOnce(Return(&sellDecision2ConfigMock));
    EXPECT_CALL(decisionMakerConfigMock, getSellDecision3Config()).WillOnce(Return(&sellDecision3ConfigMock));

    EXPECT_CALL(buyDecision1ConfigWidgetFactoryMock, newInstance(&buyDecision1ConfigMock, NotNull())).WillOnce(Return(buyDecision1ConfigWidgetMock));
    EXPECT_CALL(buyDecision2ConfigWidgetFactoryMock, newInstance(&buyDecision2ConfigMock, NotNull())).WillOnce(Return(buyDecision2ConfigWidgetMock));
    EXPECT_CALL(buyDecision3ConfigWidgetFactoryMock, newInstance(&buyDecision3ConfigMock, NotNull())).WillOnce(Return(buyDecision3ConfigWidgetMock));
    EXPECT_CALL(sellDecision1ConfigWidgetFactoryMock, newInstance(&sellDecision1ConfigMock, NotNull())).WillOnce(Return(sellDecision1ConfigWidgetMock));
    EXPECT_CALL(sellDecision2ConfigWidgetFactoryMock, newInstance(&sellDecision2ConfigMock, NotNull())).WillOnce(Return(sellDecision2ConfigWidgetMock));
    EXPECT_CALL(sellDecision3ConfigWidgetFactoryMock, newInstance(&sellDecision3ConfigMock, NotNull())).WillOnce(Return(sellDecision3ConfigWidgetMock));

    IDecisionMakerConfigWidget *widget = factory.newInstance(
        &decisionMakerConfigMock,
        &buyDecision1ConfigWidgetFactoryMock,
        &buyDecision2ConfigWidgetFactoryMock,
        &buyDecision3ConfigWidgetFactoryMock,
        &sellDecision1ConfigWidgetFactoryMock,
        &sellDecision2ConfigWidgetFactoryMock,
        &sellDecision3ConfigWidgetFactoryMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
