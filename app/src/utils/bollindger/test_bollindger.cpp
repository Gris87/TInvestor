#include "src/utils/bollindger/bollindger.h"

#include <gtest/gtest.h>



constexpr qint64 MS_IN_SECOND = 1000LL;
constexpr qint64 ONE_MINUTE   = 60LL * MS_IN_SECOND;



class Test_Bollindger : public ::testing::Test
{
protected:
    void SetUp() override
    {
        bollindger = new Bollindger();
    }

    void TearDown() override
    {
        delete bollindger;
    }

    void fillWithData(Stock* stock, QList<float> data)
    {
        stock->data.clear();

        for (int i = 0; i < data.size(); ++i)
        {
            StockData stockData;

            stockData.timestamp = QDateTime::currentMSecsSinceEpoch() - (data.size() - i - 1) * ONE_MINUTE;
            stockData.price     = data.at(i);

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

    Bollindger* bollindger;
};



TEST_F(Test_Bollindger, Test_constructor_and_destructor)
{
}

TEST_F(Test_Bollindger, Test_getTopEdge)
{
    Stock stock;

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
        }
    );

    ASSERT_NEAR(bollindger->getTopEdge(&stock, 0, stock.data.size()), 199.8075, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
        }
    );

    ASSERT_NEAR(bollindger->getTopEdge(&stock, 0, stock.data.size()), 199.7964, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
        }
    );

    ASSERT_NEAR(bollindger->getTopEdge(&stock, 0, stock.data.size()), 199.6768, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
            195.8f,
        }
    );

    ASSERT_NEAR(bollindger->getTopEdge(&stock, 0, stock.data.size()), 199.4779, 0.0001);

    ASSERT_NEAR(bollindger->getTopEdge(&stock, 0, 0), -1.0, 0.0001);
}

TEST_F(Test_Bollindger, Test_getBottomEdge)
{
    Stock stock;

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
        }
    );

    ASSERT_NEAR(bollindger->getBottomEdge(&stock, 0, stock.data.size()), 194.8325, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
        }
    );

    ASSERT_NEAR(bollindger->getBottomEdge(&stock, 0, stock.data.size()), 194.4170, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
        }
    );

    ASSERT_NEAR(bollindger->getBottomEdge(&stock, 0, stock.data.size()), 194.1499, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
            195.8f,
        }
    );

    ASSERT_NEAR(bollindger->getBottomEdge(&stock, 0, stock.data.size()), 194.0288, 0.0001);

    ASSERT_NEAR(bollindger->getBottomEdge(&stock, 0, 0), -1.0, 0.0001);
}

TEST_F(Test_Bollindger, Test_getMovingAverage)
{
    Stock stock;

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
        }
    );

    ASSERT_NEAR(bollindger->getMovingAverage(&stock, 0, stock.data.size()), 197.32, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
        }
    );

    ASSERT_NEAR(bollindger->getMovingAverage(&stock, 0, stock.data.size()), 197.1067, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
        }
    );

    ASSERT_NEAR(bollindger->getMovingAverage(&stock, 0, stock.data.size()), 196.9133, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
            195.8f,
        }
    );

    ASSERT_NEAR(bollindger->getMovingAverage(&stock, 0, stock.data.size()), 196.7533, 0.0001);
}

TEST_F(Test_Bollindger, Test_getStandartDeviation)
{
    Stock stock;

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
        }
    );

    ASSERT_NEAR(bollindger->getStandartDeviation(&stock, 0, stock.data.size(), 197.32), 1.2438, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
        }
    );

    ASSERT_NEAR(bollindger->getStandartDeviation(&stock, 0, stock.data.size(), 197.1067), 1.3449, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
        }
    );

    ASSERT_NEAR(bollindger->getStandartDeviation(&stock, 0, stock.data.size(), 196.9133), 1.3817, 0.0001);

    fillWithData(
        &stock,
        {
            198.2f,
            198.2f,
            198.1f,
            198.1f,
            197.7f,
            198.1f,
            197.7f,
            197.4f,
            196.8f,
            195.4f,
            194.9f,
            194.6f,
            195.0f,
            195.3f,
            195.8f,
        }
    );

    ASSERT_NEAR(bollindger->getStandartDeviation(&stock, 0, stock.data.size(), 196.7533), 1.3623, 0.0001);
}
