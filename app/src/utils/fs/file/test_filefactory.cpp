#include "src/utils/fs/file/filefactory.h"

#include <gtest/gtest.h>



TEST(Test_FileFactory, Test_constructor_and_destructor)
{
    const FileFactory factory;
}

TEST(Test_FileFactory, Test_newInstance)
{
    const FileFactory factory;

    const std::shared_ptr<IFile> file = factory.newInstance("test.txt");
    ASSERT_TRUE(file != nullptr);
}
