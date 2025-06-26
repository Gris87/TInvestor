#include "src/utils/fs/zip/qzip/qzipfactory.h"

#include <QBuffer>
#include <gtest/gtest.h>



TEST(Test_QZipFactory, Test_constructor_and_destructor)
{
    const QZipFactory factory;
}

TEST(Test_QZipFactory, Test_newInstance)
{
    const QZipFactory factory;

    QBuffer zipBuffer;

    const std::shared_ptr<IQZip> QZip = factory.newInstance(&zipBuffer);
    ASSERT_TRUE(QZip != nullptr);
}
