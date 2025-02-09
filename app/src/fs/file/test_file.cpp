#include "src/fs/file/file.h"

#include <gtest/gtest.h>



class Test_File : public ::testing::Test
{
protected:
    void SetUp()
    {
        file = new File();
    }

    void TearDown()
    {
        delete file;
    }

    File* file;
};



TEST_F(Test_File, Test_constructor_and_destructor)
{
}
