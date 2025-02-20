#include "src/utils/fs/file/filefactory.h"

#include <gtest/gtest.h>



TEST(Test_FileFactory, Test_constructor_and_destructor)
{
    FileFactory factory;
}

TEST(Test_FileFactory, Test_newInstance)
{
    FileFactory factory;

    std::shared_ptr<IFile> file = factory.newInstance("test.txt");
    ASSERT_TRUE(file != nullptr);
}
