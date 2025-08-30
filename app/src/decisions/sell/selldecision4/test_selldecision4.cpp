#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



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

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause =
        sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price
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

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, 100.0f, 0.04f);

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
        },
        true
    );

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 80.0f, 100.0f, 0.04f);

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
            100.0f,
            80.0f,
            80.0f,
            80.0f,
            80.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to sell because the price fall to 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD");

    // ====================================================================
    // TEST CASE: Nothing happened to the price
    // ====================================================================
    //
    //
    // -------------------------------------------------------------------X
    //

    fillWithData(&stock, {}, false);
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

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 100.0f, 0.04f);

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

    fillWithData(&stock, {}, false);
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

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 80.0f, 100.0f, 0.04f);

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

    fillWithData(&stock, {}, false);
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
            80.0f,
            80.0f,
            80.0f,
            80.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "Decided to sell because the price fall to 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD");
}
