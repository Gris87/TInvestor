#include "src/utils/tradeutils/tradeutils.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_TradeUtils : public ::testing::Test
{
protected:
    void SetUp() override
    {
        tradeUtils = new TradeUtils();
    }

    void TearDown() override
    {
        delete tradeUtils;
    }

    TradeUtils* tradeUtils;
};



TEST_F(Test_TradeUtils, Test_constructor_and_destructor)
{
}

TEST_F(Test_TradeUtils, Test_calculateAmountOfLotsToBuy)
{
    const InSequence seq;

    StrictMock<ConfigMock> configMock;

    EXPECT_CALL(configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(configMock, isLimitByTurnover()).WillOnce(Return(true));
    EXPECT_CALL(configMock, getLimitStockPurchasePart()).WillOnce(Return(7.0f));
    EXPECT_CALL(configMock, getLimitByTurnoverPercent()).WillOnce(Return(0.0005f));

    ASSERT_EQ(tradeUtils->calculateAmountOfLotsToBuy(&configMock, 100000.0, 1000000.0, 9000000000.0, 30.0, 30.012), 1500);

    EXPECT_CALL(configMock, isLimitStockPurchase()).WillOnce(Return(true));
    EXPECT_CALL(configMock, isLimitByTurnover()).WillOnce(Return(false));
    EXPECT_CALL(configMock, getLimitStockPurchasePart()).WillOnce(Return(7.0f));

    ASSERT_EQ(tradeUtils->calculateAmountOfLotsToBuy(&configMock, 100000.0, 1000000.0, 9000000000.0, 30.0, 30.012), 2333);

    EXPECT_CALL(configMock, isLimitStockPurchase()).WillOnce(Return(false));

    ASSERT_EQ(tradeUtils->calculateAmountOfLotsToBuy(&configMock, 100000.0, 1000000.0, 9000000000.0, 30.0, 30.012), 3332);
}
