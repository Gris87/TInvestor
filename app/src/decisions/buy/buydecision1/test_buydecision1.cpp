#include "src/decisions/buy/buydecision1/buydecision1.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision1 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision1 = new BuyDecision1();
    }

    void TearDown() override
    {
        delete buyDecision1;
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

    BuyDecision1* buyDecision1;
};



TEST_F(Test_BuyDecision1, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision1, Test_makeDecision)
{
    const InSequence seq;

    StrictMock<DecisionMakerConfigMock> configMock;
    StrictMock<BuyDecision1ConfigMock>  decisionConfigMock;

    Stock stock;
    stock.meta.pricePrecision = 2;

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(false));

    QString cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    /*
    fillWithData(
        &stock,
        {
            101.0f,
            101.0f,
            101.0f,
            101.0f,
            101.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    fillWithData(
        &stock,
        {
            101.0f,
            101.0f,
            150.0f,
            101.0f,
            101.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    fillWithData(
        &stock,
        {
            123.0f,
            122.0f,
            121.0f,
            120.0f,
            101.0f,
        },
        true
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, true, 4, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        "Decided to buy because the price fall to 100.00 \u20BD from 120.00 \u20BD at 2024-01-01 00:03:00 within last 3 minutes "
        "and the fall is -16.67%"
    );

    fillWithData(&stock, {}, false);
    fillWithOperationalData(
        &stock,
        {
            101.0f,
            101.0f,
            101.0f,
            101.0f,
            101.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    fillWithData(&stock, {}, false);
    fillWithOperationalData(
        &stock,
        {
            101.0f,
            101.0f,
            150.0f,
            101.0f,
            101.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    fillWithData(&stock, {}, false);
    fillWithOperationalData(
        &stock,
        {
            123.0f,
            122.0f,
            121.0f,
            120.0f,
            101.0f,
        }
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString("Decided to buy because the price fall to 100.00 \u20BD from 120.00 \u20BD at %1 within last 3 minutes and the "
                "fall is -16.67%")
            .arg(QDateTime::fromMSecsSinceEpoch(stock.operational.detailedData.at(3).timestamp).toString(DATETIME_FORMAT))
    );

    fillWithOperationalData(&stock, {});
    fillWithData(
        &stock,
        {
            101.0f,
            101.0f,
            101.0f,
            101.0f,
            101.0f,
        },
        false
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    fillWithOperationalData(&stock, {});
    fillWithData(
        &stock,
        {
            101.0f,
            101.0f,
            150.0f,
            101.0f,
            101.0f,
        },
        false
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(cause, "");

    fillWithOperationalData(&stock, {});
    fillWithData(
        &stock,
        {
            123.0f,
            122.0f,
            121.0f,
            120.0f,
            101.0f,
        },
        false
    );

    EXPECT_CALL(configMock, getBuyDecision1Config()).WillOnce(Return(&decisionConfigMock));
    EXPECT_CALL(decisionConfigMock, isEnabled()).WillOnce(Return(true));
    EXPECT_CALL(decisionConfigMock, getPriceFall()).WillOnce(Return(2.0f));
    EXPECT_CALL(decisionConfigMock, getDuration()).WillOnce(Return(3));

    cause = buyDecision1->makeDecision(QThread::currentThread(), &configMock, 0, &stock, false, -1, 100.0f, -1.0f, 0.04f);

    ASSERT_EQ(
        cause,
        QString("Decided to buy because the price fall to 100.00 \u20BD from 120.00 \u20BD at %1 within last 3 minutes and the "
                "fall is -16.67%")
            .arg(QDateTime::fromMSecsSinceEpoch(stock.data.at(3).timestamp).toString(DATETIME_FORMAT))
    );
    */
}
