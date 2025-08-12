#include "src/db/portfolio/portfoliodatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_PortfolioDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in PortfolioDatabase constructor
        dirFactoryMock               = new StrictMock<DirFactoryMock>();
        fileFactoryMock              = new StrictMock<FileFactoryMock>();
        logosStorageMock             = new StrictMock<LogosStorageMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));

        database = new PortfolioDatabase(dirFactoryMock, fileFactoryMock, logosStorageMock);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
        delete logosStorageMock;
    }

    PortfolioDatabase*            database;
    StrictMock<DirFactoryMock>*   dirFactoryMock;
    StrictMock<FileFactoryMock>*  fileFactoryMock;
    StrictMock<LogosStorageMock>* logosStorageMock;
    QString                       appDir;
};



TEST_F(Test_PortfolioDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioDatabase, Test_readPortfolio)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readPortfolio function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readPortfolio function

    Portfolio portfolio;
    Logo      logo;

    QString portfolioStr =
        R"([{"cost":"2.00","id":1,"items":[{"available":"4.00","avgPriceFifo":"6.0000000000000000","avgPriceWavg":"7.0000000000000000","cost":"8.00","costForDailyYield":"14.00","dailyYield":"12.00","dailyYieldPercent":"15.00","instrumentId":"b","instrumentName":"d","instrumentTicker":"c","part":"9.00","price":"5.0000000000000000","priceForDailyYield":"13.00","pricePrecision":16,"showPrices":true,"yield":"10.00","yieldPercent":"11.00"}],"name":"a","part":"3.00"}])";
    QByteArray portfolioBytes = portfolioStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/portfolio.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return(portfolioBytes));
    EXPECT_CALL(*fileMock1, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("b"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    portfolio = database->readPortfolio();

    // clang-format off
    ASSERT_EQ(portfolio.positions.size(),                                 1);
    ASSERT_EQ(portfolio.positions.at(0).id,                               1);
    ASSERT_EQ(portfolio.positions.at(0).name,                             "a");
    ASSERT_NEAR(portfolio.positions.at(0).cost,                           2.0, 0.0001);
    ASSERT_NEAR(portfolio.positions.at(0).part,                           3.0, 0.0001f);
    ASSERT_EQ(portfolio.positions.at(0).items.size(),                     1);
    ASSERT_EQ(portfolio.positions.at(0).items.at(0).instrumentId,         "b");
    ASSERT_EQ(portfolio.positions.at(0).items.at(0).instrumentLogo,       &logo);
    ASSERT_EQ(portfolio.positions.at(0).items.at(0).instrumentTicker,     "c");
    ASSERT_EQ(portfolio.positions.at(0).items.at(0).instrumentName,       "d");
    ASSERT_EQ(portfolio.positions.at(0).items.at(0).showPrices,           true);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).available,          4.0,   0.0001);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).price,              5.0f,  0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).avgPriceFifo,       6.0f,  0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).avgPriceWavg,       7.0f,  0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).cost,               8.0,   0.0001);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).part,               9.0f,  0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).yield,              10.0f, 0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).yieldPercent,       11.0f, 0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).dailyYield,         12.0f, 0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).priceForDailyYield, 13.0f, 0.0001f);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).costForDailyYield,  14.0,  0.0001);
    ASSERT_NEAR(portfolio.positions.at(0).items.at(0).dailyYieldPercent,  15.0f, 0.0001f);
    ASSERT_EQ(portfolio.positions.at(0).items.at(0).pricePrecision,       16);
    // clang-format on

    portfolioStr   = "{Bad content ::::: 555";
    portfolioBytes = portfolioStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/portfolio.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(portfolioBytes));
    EXPECT_CALL(*fileMock2, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, readUnlock());

    portfolio = database->readPortfolio();

    ASSERT_EQ(portfolio.positions.size(), 0);
}

TEST_F(Test_PortfolioDatabase, Test_writePortfolio)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writePortfolio function

    Portfolio portfolio;

    PortfolioCategoryItem category;

    category.id   = 1;
    category.name = "a";
    category.cost = 2.0;
    category.part = 3.0f;

    PortfolioItem item;

    item.instrumentId       = "b";
    item.instrumentTicker   = "c";
    item.instrumentName     = "d";
    item.showPrices         = true;
    item.available          = 4.0;
    item.price              = 5.0f;
    item.avgPriceFifo       = 6.0f;
    item.avgPriceWavg       = 7.0f;
    item.cost               = 8.0;
    item.part               = 9.0f;
    item.yield              = 10.0f;
    item.yieldPercent       = 11.0f;
    item.dailyYield         = 12.0f;
    item.priceForDailyYield = 13.0f;
    item.costForDailyYield  = 14.0;
    item.dailyYieldPercent  = 15.0f;
    item.pricePrecision     = 16;

    category.items << item;
    portfolio.positions << category;

    const QString portfolioStr =
        R"([{"cost":"2.00","id":1,"items":[{"available":"4.00","avgPriceFifo":"6.0000000000000000","avgPriceWavg":"7.0000000000000000","cost":"8.00","costForDailyYield":"14.00","dailyYield":"12.00","dailyYieldPercent":"15.00","instrumentId":"b","instrumentName":"d","instrumentTicker":"c","part":"9.00","price":"5.0000000000000000","priceForDailyYield":"13.00","pricePrecision":16,"showPrices":true,"yield":"10.00","yieldPercent":"11.00"}],"name":"a","part":"3.00"}])";
    const QByteArray portfolioBytes = portfolioStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/portfolio.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(portfolioBytes)).WillOnce(Return(portfolioBytes.size()));
    EXPECT_CALL(*fileMock, close());

    database->writePortfolio(portfolio);
}

TEST_F(Test_PortfolioDatabase, Test_deletePortfolio)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in deletePortfolio function

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/portfolio.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
    EXPECT_CALL(*fileMock, remove());

    database->deletePortfolio();
}
