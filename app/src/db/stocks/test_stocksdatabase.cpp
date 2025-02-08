#include "src/db/stocks/stocksdatabase.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>



class Test_StocksDatabase : public ::testing::Test
{
protected:
    void SetUp()
    {
        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/stocks").removeRecursively();

        database = new StocksDatabase();

        fillWithData();
    }

    void TearDown()
    {
        delete database;

        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db/stocks").removeRecursively();
    }

    void fillWithData()
    {
        QString stocksStr;

        for (int i = 0; i < 3; ++i)
        {
            stocksStr += QString("AZAZ%1\n").arg(i);
            stocksStr += QString("BLAH %1\n").arg(i);
        }

        QString appDir = qApp->applicationDirPath();

        QFile stocksFile(appDir + "/data/db/stocks/stocks.lst");

        ASSERT_TRUE(stocksFile.open(QIODevice::WriteOnly));

        stocksFile.write(stocksStr.toUtf8());
        stocksFile.close();



        QVariantList timestamps[3];
        QVariantList values[3];

        timestamps[0] << 100 << 200 << 300;
        values[0] << 20 << 1000 << 500;

        timestamps[1] << 150 << 250 << 350 << 450 << 550 << 650;
        values[1] << 1000.0f << 999.85f << 1000.35f << 875.95f << 1550.75f << 650.15f;

        timestamps[2] << 120 << 220 << 320 << 420 << 520;
        values[2] << 300 << 130 << 450 << 600 << 100;

        for (int i = 0; i < 3; ++i)
        {
            int              dataSize = timestamps[i].size();
            QList<StockData> stockDataList(dataSize);

            StockData* stockDataArray = stockDataList.data();

            for (int j = 0; j < dataSize; ++j)
            {
                StockData* stockData = &stockDataArray[j];

                stockData->timestamp = timestamps[i][j].toLongLong();
                stockData->value     = values[i][j].toFloat();
            }

            QFile stockDataFile(QString("%1/data/db/stocks/AZAZ%2.dat").arg(appDir).arg(i));

            ASSERT_TRUE(stockDataFile.open(QIODevice::WriteOnly));

            stockDataFile.write(reinterpret_cast<const char*>(stockDataArray), dataSize * sizeof(StockData));
            stockDataFile.close();
        }
    }

    StocksDatabase* database;
};



TEST_F(Test_StocksDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksDatabase, Test_readStocksMeta)
{
    QList<Stock> stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_readStocksData)
{
    QList<Stock> stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);
    // clang-format on

    database->readStocksData(&stocks);

    // clang-format off
    ASSERT_EQ(stocks.size(),                     3);
    ASSERT_EQ(stocks.at(0).name,                 "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,             "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),          3);
    ASSERT_EQ(stocks.at(0).data.capacity(),      1443);
    ASSERT_EQ(stocks.at(0).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks.at(0).data.at(0).value,   20, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(1).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(1).value,   1000, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(2).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(2).value,   500, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,                 "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,             "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),          6);
    ASSERT_EQ(stocks.at(1).data.capacity(),      1446);
    ASSERT_EQ(stocks.at(1).data.at(0).timestamp, 150);
    ASSERT_NEAR(stocks.at(1).data.at(0).value,   1000.0f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(1).timestamp, 250);
    ASSERT_NEAR(stocks.at(1).data.at(1).value,   999.85f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(2).timestamp, 350);
    ASSERT_NEAR(stocks.at(1).data.at(2).value,   1000.35f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(3).timestamp, 450);
    ASSERT_NEAR(stocks.at(1).data.at(3).value,   875.95f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(4).timestamp, 550);
    ASSERT_NEAR(stocks.at(1).data.at(4).value,   1550.75f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(5).timestamp, 650);
    ASSERT_NEAR(stocks.at(1).data.at(5).value,   650.15f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          5);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 120);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   300, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 220);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   130, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(2).timestamp, 320);
    ASSERT_NEAR(stocks.at(2).data.at(2).value,   450, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(3).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(3).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(4).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(4).value,   100, 0.001f);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_writeStocksMeta)
{
    QList<Stock> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stock1.name = "TEST";
    stock2.name = "MAGA";
    stock3.name = "HNYA";

    stock1.fullname = "abc";
    stock2.fullname = "def";
    stock3.fullname = "aaaa";

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.value = 0.1f;
    stockData2.value = 0.2f;
    stockData3.value = 0.3f;
    stockData4.value = 0.4f;

    stock1.data << stockData1 << stockData2;
    stock2.data << stockData2 << stockData3;
    stock3.data << stockData1 << stockData3 << stockData4;
    stocks << stock1 << stock2 << stock3;

    database->writeStocksMeta(&stocks);
    stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "TEST");
    ASSERT_EQ(stocks.at(0).fullname,        "abc");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,            "MAGA");
    ASSERT_EQ(stocks.at(1).fullname,        "def");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,            "HNYA");
    ASSERT_EQ(stocks.at(2).fullname,        "aaaa");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_appendStockData)
{
    QList<Stock> stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);
    // clang-format on

    database->readStocksData(&stocks);

    // clang-format off
    ASSERT_EQ(stocks.size(),                     3);
    ASSERT_EQ(stocks.at(0).name,                 "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,             "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),          3);
    ASSERT_EQ(stocks.at(0).data.capacity(),      1443);
    ASSERT_EQ(stocks.at(0).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks.at(0).data.at(0).value,   20, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(1).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(1).value,   1000, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(2).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(2).value,   500, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,                 "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,             "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),          6);
    ASSERT_EQ(stocks.at(1).data.capacity(),      1446);
    ASSERT_EQ(stocks.at(1).data.at(0).timestamp, 150);
    ASSERT_NEAR(stocks.at(1).data.at(0).value,   1000.0f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(1).timestamp, 250);
    ASSERT_NEAR(stocks.at(1).data.at(1).value,   999.85f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(2).timestamp, 350);
    ASSERT_NEAR(stocks.at(1).data.at(2).value,   1000.35f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(3).timestamp, 450);
    ASSERT_NEAR(stocks.at(1).data.at(3).value,   875.95f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(4).timestamp, 550);
    ASSERT_NEAR(stocks.at(1).data.at(4).value,   1550.75f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(5).timestamp, 650);
    ASSERT_NEAR(stocks.at(1).data.at(5).value,   650.15f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          5);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 120);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   300, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 220);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   130, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(2).timestamp, 320);
    ASSERT_NEAR(stocks.at(2).data.at(2).value,   450, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(3).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(3).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(4).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(4).value,   100, 0.001f);
    // clang-format on



    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.value = 0.1f;
    stockData2.value = 0.2f;
    stockData3.value = 0.3f;
    stockData4.value = 0.4f;

    Stock stock;

    stock.name = "AZAZ0";
    stock.data << stockData1 << stockData2 << stockData3 << stockData4;

    database->appendStockData(&stock);



    database->readStocksData(&stocks);

    // clang-format off
    ASSERT_EQ(stocks.size(),                     3);
    ASSERT_EQ(stocks.at(0).name,                 "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,             "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),          7);
    ASSERT_EQ(stocks.at(0).data.capacity(),      1447);
    ASSERT_EQ(stocks.at(0).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks.at(0).data.at(0).value,   20, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(1).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(1).value,   1000, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(2).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(2).value,   500, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(3).timestamp, 100);
    ASSERT_NEAR(stocks.at(0).data.at(3).value,   0.1f, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(4).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(4).value,   0.2f, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(5).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(5).value,   0.3f, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(6).timestamp, 400);
    ASSERT_NEAR(stocks.at(0).data.at(6).value,   0.4f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,                 "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,             "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),          6);
    ASSERT_EQ(stocks.at(1).data.capacity(),      1446);
    ASSERT_EQ(stocks.at(1).data.at(0).timestamp, 150);
    ASSERT_NEAR(stocks.at(1).data.at(0).value,   1000.0f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(1).timestamp, 250);
    ASSERT_NEAR(stocks.at(1).data.at(1).value,   999.85f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(2).timestamp, 350);
    ASSERT_NEAR(stocks.at(1).data.at(2).value,   1000.35f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(3).timestamp, 450);
    ASSERT_NEAR(stocks.at(1).data.at(3).value,   875.95f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(4).timestamp, 550);
    ASSERT_NEAR(stocks.at(1).data.at(4).value,   1550.75f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(5).timestamp, 650);
    ASSERT_NEAR(stocks.at(1).data.at(5).value,   650.15f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          5);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 120);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   300, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 220);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   130, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(2).timestamp, 320);
    ASSERT_NEAR(stocks.at(2).data.at(2).value,   450, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(3).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(3).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(4).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(4).value,   100, 0.001f);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_deleteObsoleteData)
{
    QList<Stock> stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);
    // clang-format on

    database->readStocksData(&stocks);

    // clang-format off
    ASSERT_EQ(stocks.size(),                     3);
    ASSERT_EQ(stocks.at(0).name,                 "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,             "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),          3);
    ASSERT_EQ(stocks.at(0).data.capacity(),      1443);
    ASSERT_EQ(stocks.at(0).data.at(0).timestamp, 100);
    ASSERT_NEAR(stocks.at(0).data.at(0).value,   20, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(1).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(1).value,   1000, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(2).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(2).value,   500, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,                 "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,             "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),          6);
    ASSERT_EQ(stocks.at(1).data.capacity(),      1446);
    ASSERT_EQ(stocks.at(1).data.at(0).timestamp, 150);
    ASSERT_NEAR(stocks.at(1).data.at(0).value,   1000.0f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(1).timestamp, 250);
    ASSERT_NEAR(stocks.at(1).data.at(1).value,   999.85f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(2).timestamp, 350);
    ASSERT_NEAR(stocks.at(1).data.at(2).value,   1000.35f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(3).timestamp, 450);
    ASSERT_NEAR(stocks.at(1).data.at(3).value,   875.95f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(4).timestamp, 550);
    ASSERT_NEAR(stocks.at(1).data.at(4).value,   1550.75f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(5).timestamp, 650);
    ASSERT_NEAR(stocks.at(1).data.at(5).value,   650.15f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          5);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 120);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   300, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 220);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   130, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(2).timestamp, 320);
    ASSERT_NEAR(stocks.at(2).data.at(2).value,   450, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(3).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(3).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(4).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(4).value,   100, 0.001f);
    // clang-format on

    database->deleteObsoleteData(200, &stocks);
    database->readStocksData(&stocks);

    // clang-format off
    ASSERT_EQ(stocks.size(),                     3);
    ASSERT_EQ(stocks.at(0).name,                 "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,             "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),          2);
    ASSERT_EQ(stocks.at(0).data.capacity(),      1443);
    ASSERT_EQ(stocks.at(0).data.at(0).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(0).value,   1000, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(1).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(1).value,   500, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,                 "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,             "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),          5);
    ASSERT_EQ(stocks.at(1).data.capacity(),      1446);
    ASSERT_EQ(stocks.at(1).data.at(0).timestamp, 250);
    ASSERT_NEAR(stocks.at(1).data.at(0).value,   999.85f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(1).timestamp, 350);
    ASSERT_NEAR(stocks.at(1).data.at(1).value,   1000.35f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(2).timestamp, 450);
    ASSERT_NEAR(stocks.at(1).data.at(2).value,   875.95f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(3).timestamp, 550);
    ASSERT_NEAR(stocks.at(1).data.at(3).value,   1550.75f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(4).timestamp, 650);
    ASSERT_NEAR(stocks.at(1).data.at(4).value,   650.15f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          4);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 220);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   130, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 320);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   450, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(2).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(2).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(3).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(3).value,   100, 0.001f);
    // clang-format on

    database->deleteObsoleteData(400, &stocks);
    database->readStocksData(&stocks);

    // clang-format off
    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 1443);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1).name,                 "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,             "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),          3);
    ASSERT_EQ(stocks.at(1).data.capacity(),      1446);
    ASSERT_EQ(stocks.at(1).data.at(0).timestamp, 450);
    ASSERT_NEAR(stocks.at(1).data.at(0).value,   875.95f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(1).timestamp, 550);
    ASSERT_NEAR(stocks.at(1).data.at(1).value,   1550.75f, 0.001f);
    ASSERT_EQ(stocks.at(1).data.at(2).timestamp, 650);
    ASSERT_NEAR(stocks.at(1).data.at(2).value,   650.15f, 0.001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          2);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   100, 0.001f);
    // clang-format on
}
