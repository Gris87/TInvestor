#include "src/storage/logos/logosstorage.h"

#include <gtest/gtest.h>

#include "src/db/logos/ilogosdatabase_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogosStorage : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logosDatabaseMock = new StrictMock<LogosDatabaseMock>();

        storage = new LogosStorage(logosDatabaseMock);
    }

    void TearDown() override
    {
        delete storage;
        delete logosDatabaseMock;
    }

    LogosStorage*                  storage;
    StrictMock<LogosDatabaseMock>* logosDatabaseMock;
};



TEST_F(Test_LogosStorage, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogosStorage, Test_readFromDatabase)
{
    const InSequence seq;

    Logos logos;

    EXPECT_CALL(*logosDatabaseMock, prepareLogos()).WillOnce(Return(logos));

    storage->readFromDatabase();
}

TEST_F(Test_LogosStorage, Test_lock_and_unlock)
{
    storage->writeLock();
    storage->writeUnlock();
    storage->readLock();
    storage->readUnlock();
}

TEST_F(Test_LogosStorage, Test_setLogo)
{
    const InSequence seq;

    QPixmap logo;

    EXPECT_CALL(*logosDatabaseMock, writeLogo(QString("aaaaa"), NotNull()));

    storage->setLogo("aaaaa", logo);

    EXPECT_CALL(*logosDatabaseMock, writeLogo(QString("aaaaa"), NotNull()));

    storage->setLogo("aaaaa", logo);
}

TEST_F(Test_LogosStorage, Test_getLogo)
{
    const InSequence seq;

    QPixmap logo;

    ASSERT_EQ(storage->getLogo(""), nullptr);

    ASSERT_NE(storage->getLogo("aaaaa"), nullptr);

    EXPECT_CALL(*logosDatabaseMock, writeLogo(QString("aaaaa"), NotNull()));

    storage->setLogo("aaaaa", logo);

    EXPECT_CALL(*logosDatabaseMock, readLogo(QString("aaaaa"), NotNull()));

    ASSERT_NE(storage->getLogo("aaaaa"), nullptr);
}
