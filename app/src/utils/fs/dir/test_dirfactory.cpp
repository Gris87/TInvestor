#include "src/utils/fs/dir/dirfactory.h"

#include <gtest/gtest.h>



class Test_DirFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new DirFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    DirFactory* factory;
};



TEST_F(Test_DirFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_DirFactory, Test_newInstance)
{
    const std::shared_ptr<IDir> dir = factory->newInstance();
    ASSERT_TRUE(dir != nullptr);
}
