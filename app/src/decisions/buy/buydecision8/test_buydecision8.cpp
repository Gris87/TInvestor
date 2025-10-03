#include "src/decisions/buy/buydecision8/buydecision8.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision8config/ibuydecision8config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



const char* const DATETIME_FORMAT = "yyyy-MM-dd hh:mm:ss";

constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision8 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision8 = new BuyDecision8();
    }

    void TearDown() override
    {
        delete buyDecision8;
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

    BuyDecision8* buyDecision8;
};



TEST_F(Test_BuyDecision8, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision8, Test_makeDecision)
{
}

TEST_F(Test_BuyDecision8, Test_asapMode)
{
    ASSERT_EQ(buyDecision8->asapMode(), ASAP_MODE_NONE);
}
