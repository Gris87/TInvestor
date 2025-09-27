#include "src/decisions/buy/buydecision5/buydecision5.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"
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



class Test_BuyDecision5 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        buyDecision5 = new BuyDecision5();
    }

    void TearDown() override
    {
        delete buyDecision5;
    }

    void fillWithData(Stock* stock, QList<float> data, bool dateRange)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            if (dateRange)
            {
                stockData.timestamp = 1704056400000 + i * ONE_DAY;
            }
            else
            {
                stockData.timestamp = QDateTime::currentMSecsSinceEpoch() - (data.size() - i - 1) * ONE_DAY;
            }

            stockData.price = data.at(i);

            stock->data.append(stockData);
        }
    }

    BuyDecision5* buyDecision5;
};



TEST_F(Test_BuyDecision5, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision5, Test_makeDecision)
{
}

TEST_F(Test_BuyDecision5, Test_asapMode)
{
    ASSERT_EQ(buyDecision5->asapMode(), ASAP_MODE_IMMEDIATELY_TRADE);
}
