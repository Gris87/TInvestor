#include "src/db/stocks/stocksdatabase.h"

#include <gtest/gtest.h>



class Test_StocksDatabase : public ::testing::Test
{
protected:
    void SetUp()
    {
        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db").removeRecursively();

        database = new StocksDatabase();

        fillWithData();
    }

    void TearDown()
    {
        delete database;

        QString appDir = qApp->applicationDirPath();
        QDir(appDir + "/data/db").removeRecursively();
    }

    void fillWithData()
    {
        ASSERT_TRUE(database->db.transaction());

        QVariantList names;
        QVariantList fullnames;

        for (int i = 0; i < 3; ++i)
        {
            qInfo() << i;

            QString stockName = QString("AZAZ%1").arg(i);
            database->createStockTable(stockName);

            names << stockName;
            fullnames << QString("BLAH %1").arg(i);
        }

        QSqlQuery query(database->db);

        query.prepare("INSERT INTO stocks (name, fullname) VALUES (?, ?);");
        query.addBindValue(names);
        query.addBindValue(fullnames);

        ASSERT_TRUE(query.execBatch());

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
            QSqlQuery query(database->db);

            query.prepare(QString("INSERT INTO AZAZ%1 (timestamp, value) VALUES (?, ?);").arg(i));
            query.addBindValue(timestamps[i]);
            query.addBindValue(values[i]);

            ASSERT_TRUE(query.execBatch());
        }

        ASSERT_TRUE(database->db.commit());
    }

    StocksDatabase *database;
};



TEST_F(Test_StocksDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksDatabase, Test_createStockTable)
{
    QStringList tables = database->db.tables();

    ASSERT_EQ(tables.size(), 5);
    ASSERT_EQ(tables.at(0),  "stocks");
    ASSERT_EQ(tables.at(1),  "sqlite_sequence");
    ASSERT_EQ(tables.at(2),  "AZAZ0");
    ASSERT_EQ(tables.at(3),  "AZAZ1");
    ASSERT_EQ(tables.at(4),  "AZAZ2");

    database->createStockTable("TEST");

    tables = database->db.tables();

    ASSERT_EQ(tables.size(), 6);
    ASSERT_EQ(tables.at(0),  "stocks");
    ASSERT_EQ(tables.at(1),  "sqlite_sequence");
    ASSERT_EQ(tables.at(2),  "AZAZ0");
    ASSERT_EQ(tables.at(3),  "AZAZ1");
    ASSERT_EQ(tables.at(4),  "AZAZ2");
    ASSERT_EQ(tables.at(5),  "TEST");
}

TEST_F(Test_StocksDatabase, Test_readStocks)
{
    QList<Stock> stocks = database->readStocks();

    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);

    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);

    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);
}

TEST_F(Test_StocksDatabase, Test_readStocksData)
{
    QList<Stock> stocks = database->readStocks();

    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);

    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);

    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);

    database->readStocksData(stocks);

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
}

TEST_F(Test_StocksDatabase, Test_deleteObsoleteData)
{
    QList<Stock> stocks = database->readStocks();

    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 0);

    ASSERT_EQ(stocks.at(1).name,            "AZAZ1");
    ASSERT_EQ(stocks.at(1).fullname,        "BLAH 1");
    ASSERT_EQ(stocks.at(1).data.size(),     0);
    ASSERT_EQ(stocks.at(1).data.capacity(), 0);

    ASSERT_EQ(stocks.at(2).name,            "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,        "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),     0);
    ASSERT_EQ(stocks.at(2).data.capacity(), 0);

    database->readStocksData(stocks);

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

    database->deleteObsoleteData(200, stocks);
    database->readStocksData(stocks);

    ASSERT_EQ(stocks.size(),                     3);
    ASSERT_EQ(stocks.at(0).name,                 "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,             "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),          2);
    ASSERT_EQ(stocks.at(0).data.capacity(),      1443);
    ASSERT_EQ(stocks.at(0).data.at(0).timestamp, 200);
    ASSERT_NEAR(stocks.at(0).data.at(0).value,   1000, 0.001f);
    ASSERT_EQ(stocks.at(0).data.at(1).timestamp, 300);
    ASSERT_NEAR(stocks.at(0).data.at(1).value,   500, 0.001f);

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

    database->deleteObsoleteData(400, stocks);
    database->readStocksData(stocks);

    ASSERT_EQ(stocks.size(),                3);
    ASSERT_EQ(stocks.at(0).name,            "AZAZ0");
    ASSERT_EQ(stocks.at(0).fullname,        "BLAH 0");
    ASSERT_EQ(stocks.at(0).data.size(),     0);
    ASSERT_EQ(stocks.at(0).data.capacity(), 1443);

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

    ASSERT_EQ(stocks.at(2).name,                 "AZAZ2");
    ASSERT_EQ(stocks.at(2).fullname,             "BLAH 2");
    ASSERT_EQ(stocks.at(2).data.size(),          2);
    ASSERT_EQ(stocks.at(2).data.capacity(),      1445);
    ASSERT_EQ(stocks.at(2).data.at(0).timestamp, 420);
    ASSERT_NEAR(stocks.at(2).data.at(0).value,   600, 0.001f);
    ASSERT_EQ(stocks.at(2).data.at(1).timestamp, 520);
    ASSERT_NEAR(stocks.at(2).data.at(1).value,   100, 0.001f);
}
