#include "src/db/bidirinfos/bidirinfosdatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"



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

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/bidirinfo")).WillOnce(Return(true));

        database = new BidirInfosDatabase(dirFactoryMock);

        fillWithData();
    }

    void fillWithData()
    {
        const QString bidirInfoStr =
            "{"
            "    \"aaaaa\": {\"spread\": 0.1, \"minYield\": 1.1, \"totalYield\": 2.1},"
            "    \"bbbbb\": {\"spread\": 0.2, \"minYield\": 1.2, \"totalYield\": 2.2},"
            "    \"ccccc\": {\"spread\": 0.3, \"minYield\": 1.3, \"totalYield\": 2.3}"
            "}";

        testBidirInfos = bidirInfoStr.toUtf8();
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
    }

    BidirInfosDatabase*          database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
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

    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return("{Bad content ::::: 555"));
    EXPECT_CALL(*fileMock1, close());

    BidirInfos bidirInfos = database->readBidirInfos(std::shared_ptr<IFile>(fileMock1));

    ASSERT_EQ(bidirInfos.size(), 0);

    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readBidirInfos

    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(testBidirInfos));
    EXPECT_CALL(*fileMock2, close());

    bidirInfos = database->readBidirInfos(std::shared_ptr<IFile>(fileMock2));

    // clang-format off
    ASSERT_EQ(bidirInfos.size(),                3);
    ASSERT_NEAR(bidirInfos["aaaaa"].spread,     0.1f, 0.0001f);
    ASSERT_NEAR(bidirInfos["aaaaa"].minYield,   1.1f, 0.0001f);
    ASSERT_NEAR(bidirInfos["aaaaa"].totalYield, 2.1f, 0.0001f);
    ASSERT_NEAR(bidirInfos["bbbbb"].spread,     0.2f, 0.0001f);
    ASSERT_NEAR(bidirInfos["bbbbb"].minYield,   1.2f, 0.0001f);
    ASSERT_NEAR(bidirInfos["bbbbb"].totalYield, 2.2f, 0.0001f);
    ASSERT_NEAR(bidirInfos["ccccc"].spread,     0.3f, 0.0001f);
    ASSERT_NEAR(bidirInfos["ccccc"].minYield,   1.3f, 0.0001f);
    ASSERT_NEAR(bidirInfos["ccccc"].totalYield, 2.3f, 0.0001f);
    // clang-format on
}
