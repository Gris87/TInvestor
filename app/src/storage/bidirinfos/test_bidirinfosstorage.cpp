#include "src/storage/bidirinfos/bidirinfosstorage.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/db/bidirinfos/ibidirinfosdatabase_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::StrictMock;



class Test_BiDirInfosStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();

        biDirInfosDatabaseMock = new StrictMock<BiDirInfosDatabaseMock>();
        fileFactoryMock        = new StrictMock<FileFactoryMock>();

        storage = new BiDirInfosStorage(biDirInfosDatabaseMock, fileFactoryMock);
    }

    void TearDown() override
    {
        delete storage;
        delete biDirInfosDatabaseMock;
        delete fileFactoryMock;
    }

    BiDirInfosStorage*                  storage;
    StrictMock<BiDirInfosDatabaseMock>* biDirInfosDatabaseMock;
    StrictMock<FileFactoryMock>*        fileFactoryMock;
    QString                             appDir;
};



TEST_F(Test_BiDirInfosStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_BiDirInfosStorage, Test_readFromDatabase_and_getBiDirInfos)
{
    const InSequence seq;

    BiDirInfos biDirInfos = storage->getBiDirInfos();
    ASSERT_EQ(biDirInfos.size(), 0);

    BiDirInfo biDirInfo1;
    BiDirInfo biDirInfo2;
    BiDirInfo biDirInfo3;

    biDirInfo1.spread     = 0.1f;
    biDirInfo1.maxSpread  = 1.1f;
    biDirInfo1.minYield   = 2.1f;
    biDirInfo1.totalYield = 3.1f;
    biDirInfo1.priority   = BIDIR_PRIORITY_LOW;

    biDirInfo2.spread     = 0.2f;
    biDirInfo2.maxSpread  = 1.2f;
    biDirInfo2.minYield   = 2.2f;
    biDirInfo2.totalYield = 3.2f;
    biDirInfo2.priority   = BIDIR_PRIORITY_NORMAL;

    biDirInfo3.spread     = 0.3f;
    biDirInfo3.maxSpread  = 1.3f;
    biDirInfo3.minYield   = 2.3f;
    biDirInfo3.totalYield = 3.3f;
    biDirInfo3.priority   = BIDIR_PRIORITY_HIGH;

    biDirInfos["aaaaa"] = biDirInfo1;
    biDirInfos["bbbbb"] = biDirInfo2;
    biDirInfos["ccccc"] = biDirInfo3;

    StrictMock<FileMock>*  fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readFromDatabase
    StrictMock<FileMock>*  fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readFromDatabase
    std::shared_ptr<IFile> filePtr1  = std::shared_ptr<IFile>(fileMock1);
    std::shared_ptr<IFile> filePtr2  = std::shared_ptr<IFile>(fileMock2);

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/bidirinfo/bidir_info.json"))).WillOnce(Return(filePtr1));
    EXPECT_CALL(*fileMock1, lastModified()).WillOnce(Return(0));
    EXPECT_CALL(*fileFactoryMock, newInstance(QString(":/assets/bidir_info.json"))).WillOnce(Return(filePtr2));
    EXPECT_CALL(*biDirInfosDatabaseMock, readBiDirInfos(filePtr2)).WillOnce(Return(biDirInfos));

    storage->readFromDatabase();
    biDirInfos = storage->getBiDirInfos();

    // clang-format off
    ASSERT_EQ(biDirInfos.size(),                3);
    ASSERT_NEAR(biDirInfos["aaaaa"].spread,     0.1f, 0.0001f);
    ASSERT_NEAR(biDirInfos["aaaaa"].maxSpread,  1.1f, 0.0001f);
    ASSERT_NEAR(biDirInfos["aaaaa"].minYield,   2.1f, 0.0001f);
    ASSERT_NEAR(biDirInfos["aaaaa"].totalYield, 3.1f, 0.0001f);
    ASSERT_EQ(biDirInfos["aaaaa"].priority,     BIDIR_PRIORITY_LOW);
    ASSERT_NEAR(biDirInfos["bbbbb"].spread,     0.2f, 0.0001f);
    ASSERT_NEAR(biDirInfos["bbbbb"].maxSpread,  1.2f, 0.0001f);
    ASSERT_NEAR(biDirInfos["bbbbb"].minYield,   2.2f, 0.0001f);
    ASSERT_NEAR(biDirInfos["bbbbb"].totalYield, 3.2f, 0.0001f);
    ASSERT_EQ(biDirInfos["bbbbb"].priority,     BIDIR_PRIORITY_NORMAL);
    ASSERT_NEAR(biDirInfos["ccccc"].spread,     0.3f, 0.0001f);
    ASSERT_NEAR(biDirInfos["ccccc"].maxSpread,  1.3f, 0.0001f);
    ASSERT_NEAR(biDirInfos["ccccc"].minYield,   2.3f, 0.0001f);
    ASSERT_NEAR(biDirInfos["ccccc"].totalYield, 3.3f, 0.0001f);
    ASSERT_EQ(biDirInfos["ccccc"].priority,     BIDIR_PRIORITY_HIGH);
    // clang-format on
}

TEST_F(Test_BiDirInfosStorage, Test_lock_and_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}
