#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision4 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision4 = new SellDecision4();
    }

    void TearDown() override
    {
        delete sellDecision4;
    }

    SellDecision4* sellDecision4;
};



TEST_F(Test_SellDecision4, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision4, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<SellDecision4ConfigMock> decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to sell because the price fall to 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD");
}
