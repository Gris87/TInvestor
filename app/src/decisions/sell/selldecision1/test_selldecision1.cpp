#include "src/decisions/sell/selldecision1/selldecision1.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision1 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision1 = new SellDecision1();
    }

    void TearDown() override
    {
        delete sellDecision1;
    }

    SellDecision1* sellDecision1;
};



TEST_F(Test_SellDecision1, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision1, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<SellDecision1ConfigMock> decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getSellDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause =
        sellDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    EXPECT_CALL(configMock, getSellDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2));

    cause = sellDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Price raise with a good yield
    // ====================================================================
    //
    //                                                                   /X
    //                                                                  /
    //                                                                 /
    //                                                                /
    // --------------------------------------------------------------/
    //

    EXPECT_CALL(configMock, getSellDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2));

    cause = sellDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to sell because the price reached 120.00 \u20BD with yield +20.00% from the price 100.00 \u20BD");
}
