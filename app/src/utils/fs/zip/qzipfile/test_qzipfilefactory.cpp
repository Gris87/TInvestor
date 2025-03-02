#include "src/utils/fs/zip/qzipfile/qzipfilefactory.h"

#include <gtest/gtest.h>



TEST(Test_QZipFileFactory, Test_constructor_and_destructor)
{
    QZipFileFactory factory;
}

TEST(Test_QZipFileFactory, Test_newInstance)
{
    QZipFileFactory factory;

    QuaZip zip;

    std::shared_ptr<IQZipFile> QZipFile = factory.newInstance(&zip);
    ASSERT_TRUE(QZipFile != nullptr);
}
