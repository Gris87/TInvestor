#include "src/utils/fs/zip/qzipfile/qzipfilefactory.h"

#include <gtest/gtest.h>



class Test_QZipFileFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new QZipFileFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    QZipFileFactory* factory;
};



TEST_F(Test_QZipFileFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_QZipFileFactory, Test_newInstance)
{
    QuaZip zip;

    const std::shared_ptr<IQZipFile> QZipFile = factory->newInstance(&zip);
    ASSERT_TRUE(QZipFile != nullptr);
}
