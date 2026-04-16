#include "src/decisions/buy/buydecision2/buydecision2.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision2 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision2 = new BuyDecision2();
    }

    void TearDown() override
    {
        delete buyDecision2;
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

    BuyDecision2* buyDecision2;
};



TEST_F(Test_BuyDecision2, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision2, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision2ConfigMock>  decisionConfigMock;

    buyDecision2->testSetStepForTripleCheck(3);

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause =
        buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: It was sold recently
    // ====================================================================

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

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));

    cause = buyDecision2->makeDecision(
        QThread::currentThread(), &configMock, 1705266000000, &stock, true, 14, false, 100.0f, -1.0f, 0.04f
    );

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

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, -1.0f, 0.04f);

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
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall but extreme rise was detected earlier
    // ====================================================================
    //
    //                                                         /---\
    //                                                         |    \---\
    //                                                        /         |
    //                                                        |          \X
    //                                                       /
    //                                                       |
    //                                                      /
    //                                                      |
    // ----------------------------------------------------/
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, -1.0f, 0.04f);

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
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, -1.0f, 0.04f);

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
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price fall to 100.00 \u20BD from 150.00 \u20BD at 2024-01-01 00:10:00 and lost yield 25.00% "
        "from the minimum price 80.00 \u20BD at 2024-01-01 00:13:00 within last 10 minutes and the fall is -33.33%"
    );

    // ====================================================================
    // TEST CASE: It was sold recently
    // ====================================================================

    fillWithData(&stock, {});
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

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));

    cause = buyDecision2->makeDecision(
        QThread::currentThread(), &configMock, QDateTime::currentMSecsSinceEpoch(), &stock, false, -1, false, 100.0f, -1.0f, 0.04f
    );

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price
    // ====================================================================
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(&stock, {});
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

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

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

    fillWithData(&stock, {});
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
            150.0f,
            150.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall but extreme rise was detected earlier
    // ====================================================================
    //
    //                                                         /---\
    //                                                         |    \---\
    //                                                        /         |
    //                                                        |          \X
    //                                                       /
    //                                                       |
    //                                                      /
    //                                                      |
    // ----------------------------------------------------/
    //

    fillWithData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            150.0f,
        }
    );
    fillWithOperationalData(
        &stock,
        {
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            150.0f,
            100.0f,
            100.0f,
            100.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

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

    fillWithData(&stock, {});
    fillWithOperationalData(
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
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

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

    fillWithData(&stock, {});
    fillWithOperationalData(
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
        }
    );

    EXPECT_CALL(configMock, getBuyDecision2Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(0.7f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));

    cause = buyDecision2->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, false, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to buy because the price fall to 100.00 \u20BD from 150.00 \u20BD at %1 and lost yield 25.00% from the "
            "minimum price 80.00 \u20BD at %2 within last 10 minutes and the fall is -33.33%"
        )
            .arg(
                QDateTime::fromMSecsSinceEpoch(stock.operational.detailedData.at(10).timestamp).toString(DATETIME_FORMAT),
                QDateTime::fromMSecsSinceEpoch(stock.operational.detailedData.at(13).timestamp).toString(DATETIME_FORMAT)
            )
    );
}

TEST_F(Test_BuyDecision2, Test_asapMode)
{
    ASSERT_EQ(buyDecision2->asapMode(), ASAP_MODE_NONE);
}
