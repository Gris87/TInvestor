#include "src/utils/fs/zip/qzipfile/qzipfilefactory.h"

#include <gtest/gtest.h>



TEST(Test_QZipFileFactory, Test_constructor_and_destructor)
{
    const QZipFileFactory factory;
}

TEST(Test_QZipFileFactory, Test_newInstance)
{
    const QZipFileFactory factory;

    QuaZip zip;

    const std::shared_ptr<IQZipFile> QZipFile = factory.newInstance(&zip);
    ASSERT_TRUE(QZipFile != nullptr);
}
