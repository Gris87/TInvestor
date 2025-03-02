#include "src/utils/fs/zip/qzip/qzipfactory.h"

#include <gtest/gtest.h>



TEST(Test_QZipFactory, Test_constructor_and_destructor)
{
    QZipFactory factory;
}

TEST(Test_QZipFactory, Test_newInstance)
{
    QZipFactory factory;

    std::shared_ptr<IQZip> QZip = factory.newInstance("test.zip");
    ASSERT_TRUE(QZip != nullptr);
}
