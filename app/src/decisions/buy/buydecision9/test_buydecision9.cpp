#include "src/decisions/buy/buydecision9/buydecision9.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision9config/ibuydecision9config_mock.h"
#include "src/config/decisions/idecisionmakerconfig_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BuyDecision9 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision9 = new BuyDecision9();
    }

    void TearDown() override
    {
        delete buyDecision9;
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

    BuyDecision9* buyDecision9;
};



TEST_F(Test_BuyDecision9, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision9, Test_makeDecision)
{
}

TEST_F(Test_BuyDecision9, Test_asapMode)
{
    ASSERT_EQ(buyDecision9->asapMode(), ASAP_MODE_FOLLOW_PRICE);
}
