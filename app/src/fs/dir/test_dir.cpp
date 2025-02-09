#include "src/fs/dir/dir.h"

#include <gtest/gtest.h>



class Test_Dir : public ::testing::Test
{
protected:
    void SetUp()
    {
        dir = new Dir();
    }

    void TearDown()
    {
        delete dir;
    }

    Dir* dir;
};



TEST_F(Test_Dir, Test_constructor_and_destructor)
{
}
