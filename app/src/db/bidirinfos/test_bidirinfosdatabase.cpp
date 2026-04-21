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



class Test_BiDirInfosDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in BiDirInfosDatabase constructor
        dirFactoryMock               = new StrictMock<DirFactoryMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/bidirinfo")).WillOnce(Return(true));

        database = new BiDirInfosDatabase(dirFactoryMock);

        fillWithData();
    }

    void fillWithData()
    {
        const QString biDirInfoStr =
            "{"
            "    \"aaaaa\": {\"spread\": 0.1, \"priority\": \"low\",    \"minYield\": 1.1, \"totalYield\": 2.1},"
            "    \"bbbbb\": {\"spread\": 0.2, \"priority\": \"normal\", \"minYield\": 1.2, \"totalYield\": 2.2},"
            "    \"ccccc\": {\"spread\": 0.3, \"priority\": \"high\",   \"minYield\": 1.3, \"totalYield\": 2.3}"
            "}";

        testBiDirInfos = biDirInfoStr.toUtf8();
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
    }

    BiDirInfosDatabase*         database;
    StrictMock<DirFactoryMock>* dirFactoryMock;
    QString                     appDir;
    QByteArray                  testBiDirInfos;
};



TEST_F(Test_BiDirInfosDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirInfosDatabase, Test_readBiDirInfos)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readBiDirInfos

    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return("{Bad content ::::: 555"));
    EXPECT_CALL(*fileMock1, close());

    BiDirInfos biDirInfos = database->readBiDirInfos(std::shared_ptr<IFile>(fileMock1));

    ASSERT_EQ(biDirInfos.size(), 0);

    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readBiDirInfos

    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(testBiDirInfos));
    EXPECT_CALL(*fileMock2, close());

    biDirInfos = database->readBiDirInfos(std::shared_ptr<IFile>(fileMock2));

    // clang-format off
    ASSERT_EQ(biDirInfos.size(),                3);
    ASSERT_NEAR(biDirInfos["aaaaa"].spread,     0.1f, 0.0001f);
    ASSERT_NEAR(biDirInfos["aaaaa"].minYield,   1.1f, 0.0001f);
    ASSERT_NEAR(biDirInfos["aaaaa"].totalYield, 2.1f, 0.0001f);
    ASSERT_EQ(biDirInfos["aaaaa"].priority,     BIDIR_PRIORITY_LOW);
    ASSERT_NEAR(biDirInfos["bbbbb"].spread,     0.2f, 0.0001f);
    ASSERT_NEAR(biDirInfos["bbbbb"].minYield,   1.2f, 0.0001f);
    ASSERT_NEAR(biDirInfos["bbbbb"].totalYield, 2.2f, 0.0001f);
    ASSERT_EQ(biDirInfos["bbbbb"].priority,     BIDIR_PRIORITY_NORMAL);
    ASSERT_NEAR(biDirInfos["ccccc"].spread,     0.3f, 0.0001f);
    ASSERT_NEAR(biDirInfos["ccccc"].minYield,   1.3f, 0.0001f);
    ASSERT_NEAR(biDirInfos["ccccc"].totalYield, 2.3f, 0.0001f);
    ASSERT_EQ(biDirInfos["ccccc"].priority,     BIDIR_PRIORITY_HIGH);
    // clang-format on
}
