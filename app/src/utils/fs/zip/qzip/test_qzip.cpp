#include "src/utils/fs/zip/qzip/qzip.h"

#include <QBuffer>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/fs/zip/qzipfile/iqzipfile.h"



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_QZip : public ::testing::Test
{
protected:
    void SetUp() override
    {
        QuaZip quaZip(&zipBuffer);
        quaZip.open(QuaZip::mdCreate);

        QuaZipFile zipFile(&quaZip);

        const QuaZipNewInfo info("hello.txt", "hello.txt");
        ASSERT_TRUE(zipFile.open(QIODevice::WriteOnly, info));
        ASSERT_EQ(zipFile.write("AzazAZazzzaaz"), 13);
        zipFile.close();

        const QuaZipNewInfo info2("world.txt", "world.txt");
        ASSERT_TRUE(zipFile.open(QIODevice::WriteOnly, info2));
        ASSERT_EQ(zipFile.write("Hello World!!!"), 14);
        zipFile.close();

        quaZip.close();

        qZip = new QZip(&zipBuffer);
    }

    void TearDown() override
    {
        delete qZip;
    }

    QZip*   qZip;
    QBuffer zipBuffer;
};



TEST_F(Test_QZip, Test_constructor_and_destructor)
{
}

TEST_F(Test_QZip, Test_getZip)
{
    ASSERT_NE(qZip->getZip(), nullptr);
}

TEST_F(Test_QZip, Test_open_getFileNameList_setCurrentFile_close)
{
    // clang-format off
    ASSERT_EQ(qZip->open(QuaZip::mdUnzip),       true);
    ASSERT_EQ(qZip->getFileNameList(),           QStringList() << "hello.txt" << "world.txt");
    ASSERT_EQ(qZip->setCurrentFile("hello.txt"), true);
    ASSERT_EQ(qZip->setCurrentFile("world.txt"), true);
    ASSERT_EQ(qZip->setCurrentFile("aaaaa.txt"), false);
    // clang-format on

    qZip->close();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
