#include "src/decisions/sell/selldecision3/selldecision3.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision3config/iselldecision3config_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

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

    StockData            stockData1;
    StockData            stockData2;
    StockData            stockData3;
    StockData            stockData4;
    StockData            stockData5;
    StockOperationalData stockOperationalData1;
    StockOperationalData stockOperationalData2;
    StockOperationalData stockOperationalData3;
    StockOperationalData stockOperationalData4;
    StockOperationalData stockOperationalData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, true, 4, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 101.0f;
    stockData2.timestamp = 1704056460000;
    stockData2.price     = 101.0f;
    stockData3.timestamp = 1704056520000;
    stockData3.price     = 101.0f;
    stockData4.timestamp = 1704056580000;
    stockData4.price     = 101.0f;
    stockData5.timestamp = 1704056640000;
    stockData5.price     = 101.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, true, 4, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 120.0f;
    stockData2.timestamp = 1704056460000;
    stockData2.price     = 120.0f;
    stockData3.timestamp = 1704056520000;
    stockData3.price     = 120.0f;
    stockData4.timestamp = 1704056580000;
    stockData4.price     = 80.0f;
    stockData5.timestamp = 1704056640000;
    stockData5.price     = 80.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, true, 4, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to sell because the price reached 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD and lost yield "
        "-33.33% from the price 120.00 \u20BD at 2024-01-01 00:02:00 within last 3 minutes"
    );

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 140.0f;
    stockData2.timestamp = 1704056460000;
    stockData2.price     = 140.0f;
    stockData3.timestamp = 1704056520000;
    stockData3.price     = 140.0f;
    stockData4.timestamp = 1704056580000;
    stockData4.price     = 120.0f;
    stockData5.timestamp = 1704056640000;
    stockData5.price     = 120.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, true, 4, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to sell because the price reached 120.00 \u20BD with yield +20.00% from the price 100.00 \u20BD and lost yield "
        "-14.29% from the price 140.00 \u20BD at 2024-01-01 00:02:00 within last 3 minutes"
    );

    stockOperationalData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 4;
    stockOperationalData1.price     = 101.0f;
    stockOperationalData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 3;
    stockOperationalData2.price     = 101.0f;
    stockOperationalData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 2;
    stockOperationalData3.price     = 101.0f;
    stockOperationalData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE;
    stockOperationalData4.price     = 101.0f;
    stockOperationalData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockOperationalData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.operational.detailedData << stockOperationalData1 << stockOperationalData2 << stockOperationalData3
                                   << stockOperationalData4 << stockOperationalData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockOperationalData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 4;
    stockOperationalData1.price     = 120.0f;
    stockOperationalData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 3;
    stockOperationalData2.price     = 120.0f;
    stockOperationalData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 2;
    stockOperationalData3.price     = 120.0f;
    stockOperationalData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE;
    stockOperationalData4.price     = 80.0f;
    stockOperationalData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockOperationalData5.price     = 80.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.operational.detailedData << stockOperationalData1 << stockOperationalData2 << stockOperationalData3
                                   << stockOperationalData4 << stockOperationalData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to sell because the price reached 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD and lost "
            "yield -33.33% from the price 120.00 \u20BD at %1 within last 3 minutes"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stockOperationalData3.timestamp).toString(DATETIME_FORMAT))
    );

    stockOperationalData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 4;
    stockOperationalData1.price     = 140.0f;
    stockOperationalData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 3;
    stockOperationalData2.price     = 140.0f;
    stockOperationalData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 2;
    stockOperationalData3.price     = 140.0f;
    stockOperationalData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE;
    stockOperationalData4.price     = 120.0f;
    stockOperationalData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockOperationalData5.price     = 120.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.operational.detailedData << stockOperationalData1 << stockOperationalData2 << stockOperationalData3
                                   << stockOperationalData4 << stockOperationalData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to sell because the price reached 120.00 \u20BD with yield +20.00% from the price 100.00 \u20BD and lost "
            "yield -14.29% from the price 140.00 \u20BD at %1 within last 3 minutes"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stockOperationalData3.timestamp).toString(DATETIME_FORMAT))
    );

    stockData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 4;
    stockData1.price     = 101.0f;
    stockData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 3;
    stockData2.price     = 101.0f;
    stockData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 2;
    stockData3.price     = 101.0f;
    stockData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE;
    stockData4.price     = 101.0f;
    stockData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 100.0f, 100.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 4;
    stockData1.price     = 120.0f;
    stockData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 3;
    stockData2.price     = 120.0f;
    stockData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 2;
    stockData3.price     = 120.0f;
    stockData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE;
    stockData4.price     = 80.0f;
    stockData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockData5.price     = 80.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 80.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to sell because the price reached 80.00 \u20BD with yield -20.00% from the price 100.00 \u20BD and lost "
            "yield -33.33% from the price 120.00 \u20BD at %1 within last 3 minutes"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stockData3.timestamp).toString(DATETIME_FORMAT))
    );

    stockData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 4;
    stockData1.price     = 140.0f;
    stockData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 3;
    stockData2.price     = 140.0f;
    stockData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE * 2;
    stockData3.price     = 140.0f;
    stockData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_MINUTE;
    stockData4.price     = 120.0f;
    stockData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockData5.price     = 120.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getSellDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getLoseYield()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = sellDecision3->makeDecision(QThread::currentThread(), &configMock, &stock, false, -1, 120.0f, 100.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to sell because the price reached 120.00 \u20BD with yield +20.00% from the price 100.00 \u20BD and lost "
            "yield -14.29% from the price 140.00 \u20BD at %1 within last 3 minutes"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stockData3.timestamp).toString(DATETIME_FORMAT))
    );
}
