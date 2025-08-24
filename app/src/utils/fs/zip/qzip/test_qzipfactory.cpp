#include "src/utils/fs/zip/qzip/qzipfactory.h"

#include <QBuffer>
#include <gtest/gtest.h>



class Test_QZipFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new QZipFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    QZipFactory* factory;
};



TEST_F(Test_QZipFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_QZipFactory, Test_newInstance)
{
    QBuffer zipBuffer;

    const std::shared_ptr<IQZip> QZip = factory->newInstance(&zipBuffer);
    ASSERT_TRUE(QZip != nullptr);
}
