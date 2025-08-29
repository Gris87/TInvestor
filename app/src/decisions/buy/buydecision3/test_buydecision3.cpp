#include "src/decisions/buy/buydecision3/buydecision3.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"
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



class Test_BuyDecision3 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision3 = new BuyDecision3();
    }

    void TearDown() override
    {
        delete buyDecision3;
    }

    BuyDecision3* buyDecision3;
};



TEST_F(Test_BuyDecision3, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision3, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision3ConfigMock>  decisionConfigMock;

    buyDecision3->testSetStep(1);

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

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 101.0f;
    stockData2.timestamp = 1704142800000;
    stockData2.price     = 101.0f;
    stockData3.timestamp = 1704229200000;
    stockData3.price     = 101.0f;
    stockData4.timestamp = 1704315600000;
    stockData4.price     = 101.0f;
    stockData5.timestamp = 1704402000000;
    stockData5.price     = 101.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 101.0f;
    stockData2.timestamp = 1704142800000;
    stockData2.price     = 101.0f;
    stockData3.timestamp = 1704229200000;
    stockData3.price     = 101.0f;
    stockData4.timestamp = 1704315600000;
    stockData4.price     = 101.0f;
    stockData5.timestamp = 1704402000000;
    stockData5.price     = 101.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 101.0f;
    stockData2.timestamp = 1704142800000;
    stockData2.price     = 101.0f;
    stockData3.timestamp = 1704229200000;
    stockData3.price     = 150.0f;
    stockData4.timestamp = 1704315600000;
    stockData4.price     = 101.0f;
    stockData5.timestamp = 1704402000000;
    stockData5.price     = 101.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = 1704056400000;
    stockData1.price     = 123.0f;
    stockData2.timestamp = 1704142800000;
    stockData2.price     = 122.0f;
    stockData3.timestamp = 1704229200000;
    stockData3.price     = 121.0f;
    stockData4.timestamp = 1704315600000;
    stockData4.price     = 120.0f;
    stockData5.timestamp = 1704402000000;
    stockData5.price     = 101.0f;
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price fall to 100.00 \u20BD from 120.00 \u20BD at 2024-01-04 00:00:00 within last 3 days "
        "and the fall is -16.67%"
    );

    stockOperationalData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 4;
    stockOperationalData1.price     = 101.0f;
    stockOperationalData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 3;
    stockOperationalData2.price     = 101.0f;
    stockOperationalData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 2;
    stockOperationalData3.price     = 101.0f;
    stockOperationalData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY;
    stockOperationalData4.price     = 101.0f;
    stockOperationalData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockOperationalData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.operational.detailedData << stockOperationalData1 << stockOperationalData2 << stockOperationalData3
                                   << stockOperationalData4 << stockOperationalData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockOperationalData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 4;
    stockOperationalData1.price     = 101.0f;
    stockOperationalData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 3;
    stockOperationalData2.price     = 101.0f;
    stockOperationalData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 2;
    stockOperationalData3.price     = 150.0f;
    stockOperationalData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY;
    stockOperationalData4.price     = 101.0f;
    stockOperationalData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockOperationalData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.operational.detailedData << stockOperationalData1 << stockOperationalData2 << stockOperationalData3
                                   << stockOperationalData4 << stockOperationalData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockOperationalData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 4;
    stockOperationalData1.price     = 123.0f;
    stockOperationalData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 3;
    stockOperationalData2.price     = 122.0f;
    stockOperationalData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 2;
    stockOperationalData3.price     = 121.0f;
    stockOperationalData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY;
    stockOperationalData4.price     = 120.0f;
    stockOperationalData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockOperationalData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.operational.detailedData << stockOperationalData1 << stockOperationalData2 << stockOperationalData3
                                   << stockOperationalData4 << stockOperationalData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to buy because the price fall to 100.00 \u20BD from 120.00 \u20BD at %1 within last 3 days and the "
            "fall is -16.67%"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stockOperationalData4.timestamp).toString(DATETIME_FORMAT))
    );

    stockData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 4;
    stockData1.price     = 101.0f;
    stockData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 3;
    stockData2.price     = 101.0f;
    stockData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 2;
    stockData3.price     = 101.0f;
    stockData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY;
    stockData4.price     = 101.0f;
    stockData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 4;
    stockData1.price     = 101.0f;
    stockData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 3;
    stockData2.price     = 101.0f;
    stockData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 2;
    stockData3.price     = 150.0f;
    stockData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY;
    stockData4.price     = 101.0f;
    stockData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    stockData1.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 4;
    stockData1.price     = 123.0f;
    stockData2.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 3;
    stockData2.price     = 122.0f;
    stockData3.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY * 2;
    stockData3.price     = 121.0f;
    stockData4.timestamp = QDateTime::currentMSecsSinceEpoch() - ONE_DAY;
    stockData4.price     = 120.0f;
    stockData5.timestamp = QDateTime::currentMSecsSinceEpoch();
    stockData5.price     = 101.0f;
    stock.operational.detailedData.clear();
    stock.data.clear();
    stock.data << stockData1 << stockData2 << stockData3 << stockData4 << stockData5;

    EXPECT_CALL(configMock, getBuyDecision3Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision3->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString(
            "Decided to buy because the price fall to 100.00 \u20BD from 120.00 \u20BD at %1 within last 3 days and the "
            "fall is -16.67%"
        )
            .arg(QDateTime::fromMSecsSinceEpoch(stockData4.timestamp).toString(DATETIME_FORMAT))
    );
}
