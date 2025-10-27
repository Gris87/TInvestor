#include "src/decisions/sell/selldecision4/selldecision4.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"
#include "src/utils/bollindger/ibollindger_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision4 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        bollindgerMock = new StrictMock<BollindgerMock>();

        sellDecision4 = new SellDecision4(bollindgerMock);
    }

    void TearDown() override
    {
        delete sellDecision4;
        delete bollindgerMock;
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

    SellDecision4*              sellDecision4;
    StrictMock<BollindgerMock>* bollindgerMock;
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
        sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 80.0f, 0.04f);

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

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getTopEdge(&stock, 3, 15)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdge(&stock, 2, 14)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdge(&stock, 1, 13)).WillOnce(Return(100.0f));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, 80.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Bollindger top edge reached
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
            70.0f,
            71.0f,
            72.0f,
            73.0f,
            74.0f,
            75.0f,
            76.0f,
            77.0f,
            78.0f,
            99.5f,
            99.6f,
            99.7f,
            99.8f,
            99.9f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getTopEdge(&stock, 3, 15)).WillOnce(Return(110.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdge(&stock, 2, 14)).WillOnce(Return(110.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdge(&stock, 1, 13)).WillOnce(Return(90.0f));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, 80.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to sell because the price 99.80 \u20BD exceeds top Bollindger edge price 90.00 \u20BD at 2024-01-01 00:12:00"
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

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getTopEdgeOperational(&stock, Ge(3), 15)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdgeOperational(&stock, Ge(2), 14)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdgeOperational(&stock, Ge(1), 13)).WillOnce(Return(100.0f));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 80.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Bollindger top edge reached
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
            70.0f,
            71.0f,
            72.0f,
            73.0f,
            74.0f,
            75.0f,
            76.0f,
            77.0f,
            78.0f,
            99.5f,
            99.6f,
            99.7f,
            99.8f,
            99.9f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getSellDecision4Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getYieldAbove()).WillOnce(Return(2));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getTopEdgeOperational(&stock, Ge(3), 15)).WillOnce(Return(110.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdgeOperational(&stock, Ge(2), 14)).WillOnce(Return(110.0f));
    EXPECT_CALL(*bollindgerMock, getTopEdgeOperational(&stock, Ge(1), 13)).WillOnce(Return(90.0f));

    cause = sellDecision4->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, 80.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString("Decided to sell because the price 99.80 \u20BD exceeds top Bollindger edge price 90.00 \u20BD at %1")
            .arg(QDateTime::fromMSecsSinceEpoch(stock.operational.detailedData.at(12).timestamp).toString(DATETIME_FORMAT))
    );
}

TEST_F(Test_SellDecision4, Test_asapMode)
{
    ASSERT_EQ(sellDecision4->asapMode(), ASAP_MODE_NONE);
}
