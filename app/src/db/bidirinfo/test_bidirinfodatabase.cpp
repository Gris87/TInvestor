#include "src/db/bidirinfo/bidirinfodatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BidirInfosDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in BidirInfosDatabase constructor
        dirFactoryMock               = new StrictMock<DirFactoryMock>();
        fileFactoryMock              = new StrictMock<FileFactoryMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/bidirinfo")).WillOnce(Return(true));

        database = new BidirInfosDatabase(dirFactoryMock, fileFactoryMock);

        fillWithData();
    }

    void fillWithData()
    {
        const QString bidirInfoStr =
            "{"
            "    \"aaaaa\": {\"ticker\": \"BANA\", \"name\": \"Banana\",       \"lot\": 1,  \"pricePrecision\": 2, "
            "\"minPriceIncrement\": {\"units\": 0, \"nano\": 100000000}},"
            "    \"bbbbb\": {\"ticker\": \"BODO\", \"name\": \"BODO NE BODO\", \"lot\": 5,  \"pricePrecision\": 3, "
            "\"minPriceIncrement\": {\"units\": 0, \"nano\": 1000000}},"
            "    \"ccccc\": {\"ticker\": \"ZARA\", \"name\": \"Zaraza\",       \"lot\": 10, \"pricePrecision\": 4, "
            "\"minPriceIncrement\": {\"units\": 0, \"nano\": 500000}}"
            "}";

        testBidirInfos = bidirInfoStr.toUtf8();
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    BidirInfosDatabase*          database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
    StrictMock<FileFactoryMock>* fileFactoryMock;
    QString                      appDir;
    QByteArray                   testBidirInfos;
};



TEST_F(Test_BidirInfosDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_BidirInfosDatabase, Test_readBidirInfos)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readBidirInfos

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/bidirinfo/bidirinfo.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));

    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return("{Bad content ::::: 555"));
    EXPECT_CALL(*fileMock1, close());

    BidirInfos bidirInfos = database->readBidirInfos();

    ASSERT_EQ(bidirInfos.size(), 0);

    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readBidirInfos

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/bidirinfo/bidirinfo.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));

    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(testBidirInfos));
    EXPECT_CALL(*fileMock2, close());

    bidirInfos = database->readBidirInfos();

    // clang-format off
    ASSERT_EQ(bidirInfos.size(),                     3);
    ASSERT_EQ(bidirInfos["aaaaa"].ticker,            "BANA");
    ASSERT_EQ(bidirInfos["aaaaa"].name,              "Banana");
    ASSERT_EQ(bidirInfos["aaaaa"].lot,               1);
    ASSERT_EQ(bidirInfos["aaaaa"].pricePrecision,    2);
    ASSERT_EQ(bidirInfos["aaaaa"].minPriceIncrement, Quotation(0, 100000000));
    ASSERT_EQ(bidirInfos["bbbbb"].ticker,            "BODO");
    ASSERT_EQ(bidirInfos["bbbbb"].name,              "BODO NE BODO");
    ASSERT_EQ(bidirInfos["bbbbb"].lot,               5);
    ASSERT_EQ(bidirInfos["bbbbb"].pricePrecision,    3);
    ASSERT_EQ(bidirInfos["bbbbb"].minPriceIncrement, Quotation(0, 1000000));
    ASSERT_EQ(bidirInfos["ccccc"].ticker,            "ZARA");
    ASSERT_EQ(bidirInfos["ccccc"].name,              "Zaraza");
    ASSERT_EQ(bidirInfos["ccccc"].lot,               10);
    ASSERT_EQ(bidirInfos["ccccc"].pricePrecision,    4);
    ASSERT_EQ(bidirInfos["ccccc"].minPriceIncrement, Quotation(0, 500000));
    // clang-format on
}
