#include "src/decisions/buy/buydecision4/buydecision4.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;
constexpr qint64 ONE_HOUR     = 60LL * ONE_MINUTE;
constexpr qint64 ONE_DAY      = 24LL * ONE_HOUR;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision4 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision4 = new BuyDecision4();
    }

    void TearDown() override
    {
        delete buyDecision4;
    }

    void fillWithData(Stock* stock, QList<float> data, bool dateRange)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            if (dateRange)
            {
                stockData.timestamp = 1704056400000 + i * ONE_DAY;
            }
            else
            {
                stockData.timestamp = QDateTime::currentMSecsSinceEpoch() - (data.size() - i - 1) * ONE_DAY;
            }

            stockData.price = data.at(i);

            stock->data.append(stockData);
        }
    }

    BuyDecision4* buyDecision4;
};



TEST_F(Test_BuyDecision4, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision4, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision4ConfigMock>  decisionConfigMock;

    buyDecision4->testSetStep(1);

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price within 10 days
    // ====================================================================
    //
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

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Unexpected maximum
    // ====================================================================
    //
    //                                                                /\
    //                                                                ||
    //                                                               /  \
    //                                                               |  |
    // -------------------------------------------------------------/    \X
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
            150.0f,
            150.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall without raise
    // ====================================================================
    //
    // ----------------------------------------------\
    //                                               |
    //                                               \-------\
    //                                                        \
    //                                                         \
    //                                                          \-----\
    //                                                                 \
    //                                                                  \
    //                                                                   \X
    //

    fillWithData(
        &stock,
        {
            160.0f,
            159.0f,
            158.0f,
            157.0f,
            156.0f,
            155.0f,
            154.0f,
            153.0f,
            152.0f,
            151.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall with raise
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
            160.0f,
            159.0f,
            158.0f,
            157.0f,
            156.0f,
            155.0f,
            154.0f,
            153.0f,
            152.0f,
            151.0f,
            150.0f,
            100.0f,
            100.0f,
            80.0f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price fall to 100.00 \u20BD from 150.00 \u20BD at 2024-01-11 00:00:00 and lost yield 25.00% "
        "from the minimum price 80.00 \u20BD at 2024-01-14 00:00:00 within last 10 days and the fall is -33.33%"
    );

    // ====================================================================
    // TEST CASE: Nothing happened to the price within 10 days
    // ====================================================================
    //
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
        false
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Unexpected maximum
    // ====================================================================
    //
    //                                                                /\
    //                                                                ||
    //                                                               /  \
    //                                                               |  |
    // -------------------------------------------------------------/    \X
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
            150.0f,
            150.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        },
        false
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall without raise
    // ====================================================================
    //
    // ----------------------------------------------\
    //                                               |
    //                                               \-------\
    //                                                        \
    //                                                         \
    //                                                          \-----\
    //                                                                 \
    //                                                                  \
    //                                                                   \X
    //

    fillWithData(
        &stock,
        {
            160.0f,
            159.0f,
            158.0f,
            157.0f,
            156.0f,
            155.0f,
            154.0f,
            153.0f,
            152.0f,
            151.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        },
        false
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall with raise
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
            160.0f,
            159.0f,
            158.0f,
            157.0f,
            156.0f,
            155.0f,
            154.0f,
            153.0f,
            152.0f,
            151.0f,
            150.0f,
            100.0f,
            100.0f,
            80.0f,
            100.0f,
        },
        false
    );

    EXPECT_CALL(configMock, getBuyDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString("Decided to buy because the price fall to 100.00 \u20BD from 150.00 \u20BD at %1 and lost yield 25.00% from the "
                "minimum price 80.00 \u20BD at %2 within last 10 days and the fall is -33.33%")
            .arg(
                QDateTime::fromMSecsSinceEpoch(stock.data.at(10).timestamp).toString(DATETIME_FORMAT),
                QDateTime::fromMSecsSinceEpoch(stock.data.at(13).timestamp).toString(DATETIME_FORMAT)
            )
    );
}
