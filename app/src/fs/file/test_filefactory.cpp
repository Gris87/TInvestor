#include "src/fs/file/filefactory.h"

#include <gtest/gtest.h>



TEST(Test_FileFactory, Test_constructor_and_destructor)
{
    FileFactory factory;
}

TEST(Test_FileFactory, Test_newInstance)
{
    FileFactory factory;

    IFile* file = factory.newInstance();
    ASSERT_TRUE(file != nullptr);

    delete file;
}
