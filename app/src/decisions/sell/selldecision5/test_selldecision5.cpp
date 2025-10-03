#include "src/decisions/sell/selldecision5/selldecision5.h"

#include <gtest/gtest.h>

#include "src/config/decisions/idecisionmakerconfig_mock.h"
#include "src/config/decisions/sell/selldecision5config/iselldecision5config_mock.h"



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_SellDecision5 : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sellDecision5 = new SellDecision5();
    }

    void TearDown() override
    {
        delete sellDecision5;
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

    SellDecision5* sellDecision5;
};



TEST_F(Test_SellDecision5, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision5, Test_makeDecision)
{
}

TEST_F(Test_SellDecision5, Test_asapMode)
{
    ASSERT_EQ(sellDecision5->asapMode(), ASAP_MODE_FOLLOW_PRICE);
}
