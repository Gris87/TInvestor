#include "src/domain/logo/logo.h"

#include <gtest/gtest.h>



class Test_Logo : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logo = new Logo();
    }

    void TearDown() override
    {
        delete logo;
    }

    Logo* logo;
};

TEST_F(Test_Logo, Test_constructor_and_destructor)
{
    ASSERT_TRUE(logo->pixmap.isNull());
}

TEST_F(Test_Logo, Test_lock_and_unlock)
{
    logo->writeLock();
    logo->writeUnlock();
    logo->readLock();
    logo->readUnlock();
}
