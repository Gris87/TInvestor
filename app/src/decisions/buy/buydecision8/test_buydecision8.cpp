#include "src/decisions/buy/buydecision8/buydecision8.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision8config/ibuydecision8config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY      = 24LL * ONE_HOUR;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision8 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision8 = new BuyDecision8();
    }

    void TearDown() override
    {
        delete buyDecision8;
    }

    void fillWithData(Stock* stock, QList<float> data, bool dateRange)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            if (dateRange)
            {
                stockData.timestamp = 1704056400000 + i * ONE_MINUTE;
            }
            else
            {
                stockData.timestamp = QDateTime::currentMSecsSinceEpoch() - (data.size() - i - 1) * ONE_MINUTE;
            }

            stockData.price = data.at(i);

            stock->data.append(stockData);
        }
    }

    void fillWithOperationalData(Stock* stock, QList<float> data)
    {
        stock->operational.detailedData.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockOperationalData stockData;

            stockData.timestamp = QDateTime::currentMSecsSinceEpoch() - (data.size() - i - 1) * ONE_MINUTE;
            stockData.price     = data.at(i);

            stock->operational.detailedData.append(stockData);
        }
    }

    BuyDecision8* buyDecision8;
};



TEST_F(Test_BuyDecision8, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision8, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision8ConfigMock>  decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

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
        },
        true
    );

    stock.data[0].timestamp -= 2 * ONE_DAY;

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to buy because the price reach market limit at 100.00 \u20BD and hold it for 10 minutes");

    // ====================================================================
    // TEST CASE: It's not a price limit
    // ====================================================================
    //
    // ------------------------------\  /---------------------------------X
    //                                \/
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
            90.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: It's not a price limit 2
    // ====================================================================
    //
    // \
    //  ------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            110.0f,
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
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: It's a price limit
    // ====================================================================
    //
    //  ------------------------------------------------------------------X
    // /
    //

    fillWithData(
        &stock,
        {
            90.0f,
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
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to buy because the price reach market limit at 100.00 \u20BD and hold it for 10 minutes");

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
        },
        true
    );

    stock.data[0].timestamp -= 2 * ONE_DAY;

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to buy because the price reach market limit at 100.00 \u20BD and hold it for 10 minutes");

    // ====================================================================
    // TEST CASE: It's not a price limit
    // ====================================================================
    //
    // ------------------------------\  /---------------------------------X
    //                                \/
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
            90.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: It's not a price limit 2
    // ====================================================================
    //
    // \
    //  ------------------------------------------------------------------X
    //

    fillWithData(
        &stock,
        {
            110.0f,
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
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: It's a price limit
    // ====================================================================
    //
    //  ------------------------------------------------------------------X
    // /
    //

    fillWithData(
        &stock,
        {
            90.0f,
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
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision8Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision8->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to buy because the price reach market limit at 100.00 \u20BD and hold it for 10 minutes");
}

TEST_F(Test_BuyDecision8, Test_asapMode)
{
    ASSERT_EQ(buyDecision8->asapMode(), ASAP_MODE_NONE);
}
