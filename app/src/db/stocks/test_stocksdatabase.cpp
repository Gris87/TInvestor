#include "src/db/stocks/stocksdatabase.h"

#include <QCoreApplication>
#include <QVariantList>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(misc-use-internal-linkage, cppcoreguidelines-avoid-const-or-ref-data-members, cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-function-size, readability-magic-numbers)
MATCHER_P2(IsMemEqual, m, size, "")
{
    return memcmp(arg, m, size) == 0;
}



class Test_StocksDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in StocksDatabase constructor
        dirFactoryMock               = new StrictMock<DirFactoryMock>();
        fileFactoryMock              = new StrictMock<FileFactoryMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/stocks/logos")).WillOnce(Return(true));

        database = new StocksDatabase(dirFactoryMock, fileFactoryMock);

        fillWithData();
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    void fillWithData()
    {
        const QString stocksStr =
            "["
            "    {"
            "        \"forQualInvestorFlag\":true,"
            "        \"lot\":1,"
            "        \"minPriceIncrement\":"
            "        {"
            "            \"nano\":100000000,"
            "            \"units\":0"
            "        },"
            "        \"name\":\"abc\","
            "        \"ticker\":\"TEST\","
            "        \"uid\":\"aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa0\""
            "    },"
            "    {"
            "        \"forQualInvestorFlag\":false,"
            "        \"lot\":10,"
            "        \"minPriceIncrement\":"
            "        {"
            "            \"nano\":500000000,"
            "            \"units\":0"
            "        },"
            "        \"name\":\"def\","
            "        \"ticker\":\"MAGA\","
            "        \"uid\":\"aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1\""
            "    },"
            "    {"
            "        \"forQualInvestorFlag\":true,"
            "        \"lot\":100,"
            "        \"minPriceIncrement\":"
            "        {"
            "            \"nano\":500000000,"
            "            \"units\":1"
            "        },"
            "        \"name\":\"aaaa\","
            "        \"ticker\":\"HNYA\","
            "        \"uid\":\"aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa2\""
            "    }"
            "]";

        testStocks = stocksStr.toUtf8();



        QVariantList timestamps[3];
        QVariantList quantities[3];
        QVariantList prices[3];

        timestamps[0] << 100 << 200 << 300;
        quantities[0] << 6 << 5 << 4;
        prices[0] << 20 << 1000 << 500;

        timestamps[1] << 150 << 250 << 350 << 450 << 550 << 650;
        quantities[1] << 10 << 20 << 30 << 40 << 50 << 60;
        prices[1] << 1000.0f << 999.85f << 1000.35f << 875.95f << 1550.75f << 650.15f;

        timestamps[2] << 120 << 220 << 320 << 420 << 520;
        quantities[2] << 7 << 8 << 2 << 1 << 5;
        prices[2] << 300 << 130 << 450 << 600 << 100;

        for (int i = 0; i < 3; ++i)
        {
            const int        dataSize = timestamps[i].size();
            QList<StockData> stockDataList(dataSize);

            StockData* stockDataArray = stockDataList.data();

            for (int j = 0; j < dataSize; ++j)
            {
                StockData* stockData = &stockDataArray[j];

                stockData->timestamp = timestamps[i][j].toLongLong();
                stockData->quantity  = quantities[i][j].toLongLong();
                stockData->price     = prices[i][j].toFloat();
            }

            testStockData[i].append(reinterpret_cast<const char*>(stockDataArray), dataSize * sizeof(StockData));
        }
    }

    StocksDatabase*              database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
    StrictMock<FileFactoryMock>* fileFactoryMock;
    QString                      appDir;
    QByteArray                   testStocks;
    QByteArray                   testStockData[3];
};



TEST_F(Test_StocksDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksDatabase, Test_readStocksMeta)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in readStocksMeta

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/stocks/stocks.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, readAll()).WillOnce(Return(testStocks));
    EXPECT_CALL(*fileMock, close());

    const QList<Stock*> stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa0");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    ASSERT_EQ(stocks.at(0)->data.capacity(),                  0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      0);
    ASSERT_EQ(stocks.at(1)->data.capacity(),                  0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa2");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      0);
    ASSERT_EQ(stocks.at(2)->data.capacity(),                  0);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_readStocksData)
{
    // const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in readStocksMeta

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/stocks/stocks.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, readAll()).WillOnce(Return(testStocks));
    EXPECT_CALL(*fileMock, close());

    QList<Stock*> stocks = database->readStocksMeta();

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa0");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      0);
    ASSERT_EQ(stocks.at(0)->data.capacity(),                  0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      0);
    ASSERT_EQ(stocks.at(1)->data.capacity(),                  0);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa2");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      0);
    ASSERT_EQ(stocks.at(2)->data.capacity(),                  0);
    // clang-format on

    StrictMock<FileMock>* fileMocks[3];

    for (int i = 0; i < 3; ++i)
    {
        fileMocks[i] = new StrictMock<FileMock>(); // Will be deleted in readStocksData

        EXPECT_CALL(
            *fileFactoryMock, newInstance(QString("%1/data/stocks/aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa%2.dat").arg(appDir).arg(i))
        )
            .WillOnce(Return(std::shared_ptr<IFile>(fileMocks[i])));

        EXPECT_CALL(*fileMocks[i], open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
        EXPECT_CALL(*fileMocks[i], size()).WillOnce(Return(testStockData[i].size()));
        EXPECT_CALL(*fileMocks[i], read(NotNull(), testStockData[i].size())).WillOnce(Return(testStockData[i].size()));
        EXPECT_CALL(*fileMocks[i], close());
    }

    database->readStocksData(stocks);

    for (int i = 0; i < 3; ++i)
    {
        Stock* stock = stocks[i];

        if (!stock->data.isEmpty())
        {
            memcpy(stock->data.data(), testStockData[i].constData(), testStockData[i].size());
            stock->operational.lastStoredTimestamp = stock->data.last().timestamp;
        }
    }

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa0");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.capacity(),                  1443);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              6);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               20, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             300);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               500, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  650);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      6);
    ASSERT_EQ(stocks.at(1)->data.capacity(),                  1446);
    ASSERT_EQ(stocks.at(1)->data.at(0).timestamp,             150);
    ASSERT_EQ(stocks.at(1)->data.at(0).quantity,              10);
    ASSERT_NEAR(stocks.at(1)->data.at(0).price,               1000.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(1).timestamp,             250);
    ASSERT_EQ(stocks.at(1)->data.at(1).quantity,              20);
    ASSERT_NEAR(stocks.at(1)->data.at(1).price,               999.85f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(2).timestamp,             350);
    ASSERT_EQ(stocks.at(1)->data.at(2).quantity,              30);
    ASSERT_NEAR(stocks.at(1)->data.at(2).price,               1000.35f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(3).timestamp,             450);
    ASSERT_EQ(stocks.at(1)->data.at(3).quantity,              40);
    ASSERT_NEAR(stocks.at(1)->data.at(3).price,               875.95f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(4).timestamp,             550);
    ASSERT_EQ(stocks.at(1)->data.at(4).quantity,              50);
    ASSERT_NEAR(stocks.at(1)->data.at(4).price,               1550.75f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->data.at(5).timestamp,             650);
    ASSERT_EQ(stocks.at(1)->data.at(5).quantity,              60);
    ASSERT_NEAR(stocks.at(1)->data.at(5).price,               650.15f, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa2");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      5);
    ASSERT_EQ(stocks.at(2)->data.capacity(),                  1445);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             120);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              7);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               300, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             220);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              8);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             320);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              2);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             420);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(4).timestamp,             520);
    ASSERT_EQ(stocks.at(2)->data.at(4).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(4).price,               100, 0.0001f);
    // clang-format on



    for (int i = 0; i < 3; ++i)
    {
        fileMocks[i] = new StrictMock<FileMock>(); // Will be deleted in readStocksData

        EXPECT_CALL(
            *fileFactoryMock, newInstance(QString("%1/data/stocks/aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa%2.dat").arg(appDir).arg(i))
        )
            .WillOnce(Return(std::shared_ptr<IFile>(fileMocks[i])));

        if (i != 1)
        {
            EXPECT_CALL(*fileMocks[i], open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
            EXPECT_CALL(*fileMocks[i], size()).WillOnce(Return(testStockData[i].size()));
            EXPECT_CALL(*fileMocks[i], read(NotNull(), testStockData[i].size())).WillOnce(Return(testStockData[i].size()));
            EXPECT_CALL(*fileMocks[i], close());
        }
        else
        {
            EXPECT_CALL(*fileMocks[i], open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(false));
        }
    }

    database->readStocksData(stocks);

    for (int i = 0; i < 3; ++i)
    {
        Stock* stock = stocks[i];

        if (!stock->data.isEmpty())
        {
            memcpy(stock->data.data(), testStockData[i].constData(), testStockData[i].size());
            stock->operational.lastStoredTimestamp = stock->data.last().timestamp;
        }
    }

    // clang-format off
    ASSERT_EQ(stocks.size(),                                  3);
    ASSERT_EQ(stocks.at(0)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa0");
    ASSERT_EQ(stocks.at(0)->meta.ticker,                      "TEST");
    ASSERT_EQ(stocks.at(0)->meta.name,                        "abc");
    ASSERT_EQ(stocks.at(0)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(0)->meta.lot,                         1);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(0)->meta.minPriceIncrement.nano,      100000000);
    ASSERT_EQ(stocks.at(0)->operational.lastStoredTimestamp,  300);
    ASSERT_NEAR(stocks.at(0)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(0)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(0)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(0)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(0)->data.size(),                      3);
    ASSERT_EQ(stocks.at(0)->data.capacity(),                  1443);
    ASSERT_EQ(stocks.at(0)->data.at(0).timestamp,             100);
    ASSERT_EQ(stocks.at(0)->data.at(0).quantity,              6);
    ASSERT_NEAR(stocks.at(0)->data.at(0).price,               20, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(1).timestamp,             200);
    ASSERT_EQ(stocks.at(0)->data.at(1).quantity,              5);
    ASSERT_NEAR(stocks.at(0)->data.at(1).price,               1000, 0.0001f);
    ASSERT_EQ(stocks.at(0)->data.at(2).timestamp,             300);
    ASSERT_EQ(stocks.at(0)->data.at(2).quantity,              4);
    ASSERT_NEAR(stocks.at(0)->data.at(2).price,               500, 0.0001f);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(1)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa1");
    ASSERT_EQ(stocks.at(1)->meta.ticker,                      "MAGA");
    ASSERT_EQ(stocks.at(1)->meta.name,                        "def");
    ASSERT_EQ(stocks.at(1)->meta.forQualInvestorFlag,         false);
    ASSERT_EQ(stocks.at(1)->meta.lot,                         10);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.units,     0);
    ASSERT_EQ(stocks.at(1)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(1)->operational.lastStoredTimestamp,  0);
    ASSERT_NEAR(stocks.at(1)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(1)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(1)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(1)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(1)->data.size(),                      0);
    ASSERT_EQ(stocks.at(1)->data.capacity(),                  1446);
    // clang-format on

    // clang-format off
    ASSERT_EQ(stocks.at(2)->meta.uid,                         "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaa2");
    ASSERT_EQ(stocks.at(2)->meta.ticker,                      "HNYA");
    ASSERT_EQ(stocks.at(2)->meta.name,                        "aaaa");
    ASSERT_EQ(stocks.at(2)->meta.forQualInvestorFlag,         true);
    ASSERT_EQ(stocks.at(2)->meta.lot,                         100);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.units,     1);
    ASSERT_EQ(stocks.at(2)->meta.minPriceIncrement.nano,      500000000);
    ASSERT_EQ(stocks.at(2)->operational.lastStoredTimestamp,  520);
    ASSERT_NEAR(stocks.at(2)->operational.dayStartPrice,      -1.0f, 0.0001f);
    ASSERT_NEAR(stocks.at(2)->operational.specifiedDatePrice, -1.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.turnover,             0);
    ASSERT_NEAR(stocks.at(2)->operational.payback,            0.0f, 0.0001f);
    ASSERT_EQ(stocks.at(2)->operational.detailedData.size(),  0);
    ASSERT_EQ(stocks.at(2)->data.size(),                      5);
    ASSERT_EQ(stocks.at(2)->data.capacity(),                  1445);
    ASSERT_EQ(stocks.at(2)->data.at(0).timestamp,             120);
    ASSERT_EQ(stocks.at(2)->data.at(0).quantity,              7);
    ASSERT_NEAR(stocks.at(2)->data.at(0).price,               300, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(1).timestamp,             220);
    ASSERT_EQ(stocks.at(2)->data.at(1).quantity,              8);
    ASSERT_NEAR(stocks.at(2)->data.at(1).price,               130, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(2).timestamp,             320);
    ASSERT_EQ(stocks.at(2)->data.at(2).quantity,              2);
    ASSERT_NEAR(stocks.at(2)->data.at(2).price,               450, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(3).timestamp,             420);
    ASSERT_EQ(stocks.at(2)->data.at(3).quantity,              1);
    ASSERT_NEAR(stocks.at(2)->data.at(3).price,               600, 0.0001f);
    ASSERT_EQ(stocks.at(2)->data.at(4).timestamp,             520);
    ASSERT_EQ(stocks.at(2)->data.at(4).quantity,              5);
    ASSERT_NEAR(stocks.at(2)->data.at(4).price,               100, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_writeStocksMeta)
{
    const InSequence seq;

    QList<Stock*> stocks;

    Stock stock1;
    Stock stock2;
    Stock stock3;

    stock1.meta.uid                     = "aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa";
    stock1.meta.ticker                  = "TEST";
    stock1.meta.name                    = "abc";
    stock1.meta.forQualInvestorFlag     = true;
    stock1.meta.lot                     = 1;
    stock1.meta.minPriceIncrement.units = 0;
    stock1.meta.minPriceIncrement.nano  = 100000000;
    stock2.meta.uid                     = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock2.meta.ticker                  = "MAGA";
    stock2.meta.name                    = "def";
    stock2.meta.forQualInvestorFlag     = false;
    stock2.meta.lot                     = 10;
    stock2.meta.minPriceIncrement.units = 0;
    stock2.meta.minPriceIncrement.nano  = 500000000;
    stock3.meta.uid                     = "cccccccc-cccc-cccc-cccc-cccccccccccc";
    stock3.meta.ticker                  = "HNYA";
    stock3.meta.name                    = "aaaa";
    stock3.meta.forQualInvestorFlag     = true;
    stock3.meta.lot                     = 100;
    stock3.meta.minPriceIncrement.units = 1;
    stock3.meta.minPriceIncrement.nano  = 500000000;

    stocks << &stock1 << &stock2 << &stock3;

    const QString stocksStr =
        "[{\"forQualInvestorFlag\":true,\"lot\":1,\"minPriceIncrement\":{\"nano\":100000000,\"units\":0},\"name\":\"abc\","
        "\"ticker\":\"TEST\",\"uid\":\"aaaaaaaa-aaaa-aaaa-aaaa-aaaaaaaaaaaa\"},{\"forQualInvestorFlag\":false,\"lot\":10,"
        "\"minPriceIncrement\":{\"nano\":500000000,\"units\":0},\"name\":\"def\",\"ticker\":\"MAGA\",\"uid\":\"bbbbbbbb-bbbb-"
        "bbbb-bbbb-bbbbbbbbbbbb\"},{\"forQualInvestorFlag\":true,\"lot\":100,\"minPriceIncrement\":{\"nano\":500000000,\"units\":"
        "1},\"name\":\"aaaa\",\"ticker\":\"HNYA\",\"uid\":\"cccccccc-cccc-cccc-cccc-cccccccccccc\"}]";
    const QByteArray stocksBytes = stocksStr.toUtf8();

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writeStocksMeta

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/stocks/stocks.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(stocksBytes)).WillOnce(Return(stocksBytes.size()));
    EXPECT_CALL(*fileMock, close());

    database->writeStocksMeta(stocks);
}

TEST_F(Test_StocksDatabase, Test_appendStockData)
{
    const InSequence seq;

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 3;
    stockData4.quantity = 4;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;

    Stock            stock;
    QList<StockData> data;

    stock.meta.uid = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock.data << stockData1;
    data << stockData2 << stockData3 << stockData4;

    const qint64 fileSize = data.size() * sizeof(StockData);

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writeStocksMeta

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/stocks/bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb.dat")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::Append))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(IsMemEqual(data.constData(), fileSize), fileSize)).WillOnce(Return(fileSize));
    EXPECT_CALL(*fileMock, close());

    database->appendStockData(&stock, data.constData(), data.size());

    // clang-format off
    ASSERT_EQ(stock.data.size(),          4);
    ASSERT_EQ(stock.data.at(0).timestamp, 100);
    ASSERT_EQ(stock.data.at(0).quantity,  1);
    ASSERT_NEAR(stock.data.at(0).price,   0.1f, 0.0001f);
    ASSERT_EQ(stock.data.at(1).timestamp, 200);
    ASSERT_EQ(stock.data.at(1).quantity,  2);
    ASSERT_NEAR(stock.data.at(1).price,   0.2f, 0.0001f);
    ASSERT_EQ(stock.data.at(2).timestamp, 300);
    ASSERT_EQ(stock.data.at(2).quantity,  3);
    ASSERT_NEAR(stock.data.at(2).price,   0.3f, 0.0001f);
    ASSERT_EQ(stock.data.at(3).timestamp, 400);
    ASSERT_EQ(stock.data.at(3).quantity,  4);
    ASSERT_NEAR(stock.data.at(3).price,   0.4f, 0.0001f);
    // clang-format on
}

TEST_F(Test_StocksDatabase, Test_writeStockData)
{
    const InSequence seq;

    StockData stockData1;
    StockData stockData2;
    StockData stockData3;
    StockData stockData4;

    stockData1.timestamp = 100;
    stockData2.timestamp = 200;
    stockData3.timestamp = 300;
    stockData4.timestamp = 400;

    stockData1.quantity = 1;
    stockData2.quantity = 2;
    stockData3.quantity = 2;
    stockData4.quantity = 3;

    stockData1.price = 0.1f;
    stockData2.price = 0.2f;
    stockData3.price = 0.3f;
    stockData4.price = 0.4f;

    Stock stock;

    stock.meta.uid = "bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb";
    stock.data << stockData1 << stockData2 << stockData3 << stockData4;

    const qint64 fileSize = stock.data.size() * sizeof(StockData);

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writeStocksMeta

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/stocks/bbbbbbbb-bbbb-bbbb-bbbb-bbbbbbbbbbbb.dat")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(IsMemEqual(stock.data.constData(), fileSize), fileSize)).WillOnce(Return(fileSize));
    EXPECT_CALL(*fileMock, close());

    database->writeStockData(stock);
}
// NOLINTEND(misc-use-internal-linkage, cppcoreguidelines-avoid-const-or-ref-data-members, cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity, readability-function-size, readability-magic-numbers)
