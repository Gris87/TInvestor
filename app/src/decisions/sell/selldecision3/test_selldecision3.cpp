#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision3 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision3 = new SellDecision3();
    }

    void TearDown() override
    {
        delete sellDecision3;
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

    SellDecision3* sellDecision3;
};



TEST_F(Test_SellDecision3, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision3, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<SellDecision3ConfigMock> decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause =
        sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 100.0f, 0.04f);

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

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall without double check
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
            80.0f,
            80.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 80.0f, 100.0f, 0.04f);

    // ====================================================================
    // TEST CASE: Normal fall with double check
    // ====================================================================
    //
    // --------------------------------------------\
    //                                             |
    //                                             \-------\
    //                                                      \
    //                                                       \
    //                                                        \-----\
    //                                                               \
    //                                                                \
    //                                                                 \--X
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
            80.0f,
            80.0f,
            80.0f,
            80.0f,
            80.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to sell because the price fall to 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD");

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

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Normal fall without double check
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
            80.0f,
            80.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 80.0f, 100.0f, 0.04f);

    // ====================================================================
    // TEST CASE: Normal fall with double check
    // ====================================================================
    //
    // --------------------------------------------\
    //                                             |
    //                                             \-------\
    //                                                      \
    //                                                       \
    //                                                        \-----\
    //                                                               \
    //                                                                \
    //                                                                 \--X
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
            80.0f,
            80.0f,
            80.0f,
            80.0f,
            80.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to sell because the price fall to 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD");
}

TEST_F(Test_SellDecision3, Test_asapMode)
{
    ASSERT_EQ(sellDecision3->asapMode(), ASAP_MODE_FOLLOW_PRICE);
}
