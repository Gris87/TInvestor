#include "src/decisions/sell/selldecision2/selldecision2.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision2 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision2 = new SellDecision2();
    }

    void TearDown() override
    {
        delete sellDecision2;
    }

    void fillWithData(Stock* stock, QList<float> data)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            stockData.timestamp = 1704056400000 + i * ONE_MINUTE;
            stockData.price     = data.at(i);

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

    SellDecision2* sellDecision2;
};



TEST_F(Test_SellDecision2, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision2, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<SellDecision2ConfigMock> decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause =
        sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, 100.0f, 0.04f);

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

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2.0f));

    cause = sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: TEST CASE: Normal raise without fall
    // ====================================================================
    //
    //                                                                   /X
    //                                                                  /
    //                                                                 /
    //                                                                /
    // --------------------------------------------------------------/
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
            120.0f,
            120.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));

    cause = sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: TEST CASE: Normal raise with fall
    // ====================================================================
    //
    //                                                                /\
    //                                                               /  \
    //                                                              /    \X
    //                                                             /
    // -----------------------------------------------------------/
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
            124.5f,
            124.6f,
            124.7f,
            124.8f,
            124.9f,
            125.0f,
            120.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));

    cause = sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to sell because the price reached 120.00 \u20BD with yield +20.00% from the price 100.00 \u20BD and lost yield "
        "-4.00% from the maximum price 125.00 \u20BD at 2024-01-01 00:13:00"
    );

    // ====================================================================
    // TEST CASE: Nothing happened to the price
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithOperationalData(
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

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2.0f));

    cause =
        sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: TEST CASE: Normal raise without fall
    // ====================================================================
    //
    //                                                                   /X
    //                                                                  /
    //                                                                 /
    //                                                                /
    // --------------------------------------------------------------/
    //

    fillWithOperationalData(
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
            120.0f,
            120.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));

    cause =
        sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: TEST CASE: Normal raise with fall
    // ====================================================================
    //
    //                                                                /\
    //                                                               /  \
    //                                                              /    \X
    //                                                             /
    // -----------------------------------------------------------/
    //

    fillWithOperationalData(
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
            124.5f,
            124.6f,
            124.7f,
            124.8f,
            124.9f,
            125.0f,
            120.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));

    cause =
        sellDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to sell because the price reached 120.00 \u20BD with yield +20.00% from the price 100.00 \u20BD and lost "
            "yield -4.00% from the maximum price 125.00 \u20BD at %1"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stock.operational.detailedData.at(13).timestamp).toString(DATETIME_FORMAT))
    );
}

TEST_F(Test_SellDecision2, Test_asapMode)
{
    ASSERT_EQ(sellDecision2->asapMode(), ASAP_MODE_NONE);
}
