#include "src/utils/fs/file/filefactory.h"

#include <gtest/gtest.h>



class Test_FileFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new FileFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    FileFactory* factory;
};



TEST_F(Test_FileFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_FileFactory, Test_newInstance)
{
    const std::shared_ptr<IFile> file = factory->newInstance("test.txt");
    ASSERT_TRUE(file != nullptr);
}
