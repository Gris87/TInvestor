#include "src/decisions/buy/buydecision5/buydecision5.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/utils/bollindger/ibollindger_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision5 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        bollindgerMock = new StrictMock<BollindgerMock>();

        buyDecision5 = new BuyDecision5(bollindgerMock);
    }

    void TearDown() override
    {
        delete buyDecision5;
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

    BuyDecision5*               buyDecision5;
    StrictMock<BollindgerMock>* bollindgerMock;
};



TEST_F(Test_BuyDecision5, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision5, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision5ConfigMock>  decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    // ====================================================================
    // TEST CASE: Decision is disabled
    // ====================================================================

    EXPECT_CALL(configMock, getBuyDecision5Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = buyDecision5->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Nothing happened to the price within 10 minutes
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

    EXPECT_CALL(configMock, getBuyDecision5Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getBottomEdge(&stock, 3, 15)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdge(&stock, 2, 14)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdge(&stock, 1, 13)).WillOnce(Return(100.0f));

    cause = buyDecision5->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Bollindger bottom edge reached
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
            130.0f,
            125.0f,
            120.0f,
            115.0f,
            110.0f,
            109.0f,
            108.0f,
            107.0f,
            106.0f,
            105.0f,
            104.0f,
            103.0f,
            102.0f,
            101.0f,
            100.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision5Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getBottomEdge(&stock, 3, 15)).WillOnce(Return(90.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdge(&stock, 2, 14)).WillOnce(Return(90.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdge(&stock, 1, 13)).WillOnce(Return(110.0f));

    cause = buyDecision5->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 14, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price 102.00 \u20BD exceeds bottom Bollindger edge price 110.00 \u20BD at 2024-01-01 00:12:00"
    );

    // ====================================================================
    // TEST CASE: Nothing happened to the price within 10 minutes
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

    EXPECT_CALL(configMock, getBuyDecision5Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getBottomEdgeOperational(&stock, Ge(3), 15)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdgeOperational(&stock, Ge(2), 14)).WillOnce(Return(100.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdgeOperational(&stock, Ge(1), 13)).WillOnce(Return(100.0f));

    cause = buyDecision5->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    // ====================================================================
    // TEST CASE: Bollindger bottom edge reached
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
            130.0f,
            125.0f,
            120.0f,
            115.0f,
            110.0f,
            109.0f,
            108.0f,
            107.0f,
            106.0f,
            105.0f,
            104.0f,
            103.0f,
            102.0f,
            101.0f,
            100.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision5Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(10));
    EXPECT_CALL(*bollindgerMock, getBottomEdgeOperational(&stock, Ge(3), 15)).WillOnce(Return(90.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdgeOperational(&stock, Ge(2), 14)).WillOnce(Return(90.0f));
    EXPECT_CALL(*bollindgerMock, getBottomEdgeOperational(&stock, Ge(1), 13)).WillOnce(Return(110.0f));

    cause = buyDecision5->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString("Decided to buy because the price 102.00 \u20BD exceeds bottom Bollindger edge price 110.00 \u20BD at %1")
            .arg(QDateTime::fromMSecsSinceEpoch(stock.operational.detailedData.at(12).timestamp).toString(DATETIME_FORMAT))
    );
}

TEST_F(Test_BuyDecision5, Test_asapMode)
{
    ASSERT_EQ(buyDecision5->asapMode(), ASAP_MODE_NONE);
}
