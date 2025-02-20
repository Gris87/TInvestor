#include "src/utils/fs/dir/dirfactory.h"

#include <gtest/gtest.h>



TEST(Test_DirFactory, Test_constructor_and_destructor)
{
    DirFactory factory;
}

TEST(Test_DirFactory, Test_newInstance)
{
    DirFactory factory;

    std::shared_ptr<IDir> dir = factory.newInstance();
    ASSERT_TRUE(dir != nullptr);
}
