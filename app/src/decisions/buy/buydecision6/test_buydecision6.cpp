#include "src/decisions/buy/buydecision6/buydecision6.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision6config/ibuydecision6config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY      = 24LL * ONE_HOUR;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision6 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision6 = new BuyDecision6();
    }

    void TearDown() override
    {
        delete buyDecision6;
    }

    void fillWithData(Stock* stock, QList<float> data)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            stockData.timestamp = 1704056400000 + i * ONE_DAY;
            stockData.price     = data.at(i);

            stock->data.append(stockData);
        }
    }

    BuyDecision6* buyDecision6;
};



TEST_F(Test_BuyDecision6, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision6, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision6ConfigMock>  decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause =
        buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Not enough data
    // ====================================================================
    //
    // ----X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getRsi()).WillOnce(Return(20.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getRsi()).WillOnce(Return(20.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Price more than SMA and EMA and RSI is low
    // ====================================================================
    //
    // -------------------------------------------\
    //                                            |
    //                                            \-------\
    //                                                     \
    //                                                      \
    //                                                       \-----\
    //                                                              \    /X
    //                                                               \  /
    //                                                                \/
    //

    fillWithData(
        &stock,
        {
            200.0f,
            170.0f,
            160.0f,
            150.0f,
            155.0f,
            120.0f,
            123.0f,
            117.0f,
            115.0f,
            118.0f,
            114.0f,
            115.0f,
            110.0f,
            100.0f,
            130.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getRsi()).WillOnce(Return(20.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 130.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price 130.00 \u20BD is above SMA 118.70 \u20BD and EMA 124.25 \u20BD and RSI 10.14% is less "
        "than 20.00% for last 10 minutes"
    );

    // ====================================================================
    // TEST CASE: Not enough data
    // ====================================================================
    //
    // ----X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getRsi()).WillOnce(Return(20.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getRsi()).WillOnce(Return(20.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Price more than SMA and EMA and RSI is low
    // ====================================================================
    //
    // -------------------------------------------\
    //                                            |
    //                                            \-------\
    //                                                     \
    //                                                      \
    //                                                       \-----\
    //                                                              \    /X
    //                                                               \  /
    //                                                                \/
    //

    fillWithData(
        &stock,
        {
            200.0f,
            170.0f,
            160.0f,
            150.0f,
            155.0f,
            120.0f,
            123.0f,
            117.0f,
            115.0f,
            118.0f,
            114.0f,
            115.0f,
            110.0f,
            100.0f,
            130.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision6Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getRsi()).WillOnce(Return(20.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision6->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 130.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price 130.00 \u20BD is above SMA 118.70 \u20BD and EMA 124.25 \u20BD and RSI 10.14% is less "
        "than 20.00% for last 10 minutes"
    );
}

TEST_F(Test_BuyDecision6, Test_asapMode)
{
    ASSERT_EQ(buyDecision6->asapMode(), ASAP_MODE_FOLLOW_PRICE);
}
