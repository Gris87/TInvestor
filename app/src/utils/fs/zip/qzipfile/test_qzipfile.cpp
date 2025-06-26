#include "src/utils/fs/zip/qzipfile/qzipfile.h"

#include <QBuffer>
#include <QDir>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_QZipFile : public ::testing::Test
{
protected:
    void SetUp() override
    {
        zip = new QuaZip(&zipBuffer);
        zip->open(QuaZip::mdCreate);

        QuaZipFile zipFile(zip);

        const QuaZipNewInfo info("hello.txt", "hello.txt");
        ASSERT_TRUE(zipFile.open(QIODevice::WriteOnly, info));
        ASSERT_EQ(zipFile.write("AzazAZazzzaaz"), 13);
        zipFile.close();

        const QuaZipNewInfo info2("world.txt", "world.txt");
        ASSERT_TRUE(zipFile.open(QIODevice::WriteOnly, info2));
        ASSERT_EQ(zipFile.write("Hello World!!!"), 14);
        zipFile.close();

        zip->close();
        zip->open(QuaZip::mdUnzip);

        qZipFile = new QZipFile(zip);
    }

    void TearDown() override
    {
        zip->close();

        delete qZipFile;
        delete zip;
    }

    QZipFile* qZipFile;
    QuaZip*   zip;
    QBuffer   zipBuffer;
};



TEST_F(Test_QZipFile, Test_constructor_and_destructor)
{
}

TEST_F(Test_QZipFile, Test_open_readAll_close)
{
    zip->setCurrentFile("hello.txt");

    ASSERT_EQ(qZipFile->open(QIODevice::ReadOnly), true);
    ASSERT_EQ(qZipFile->readAll(), "AzazAZazzzaaz");
    qZipFile->close();

    zip->setCurrentFile("world.txt");

    ASSERT_EQ(qZipFile->open(QIODevice::ReadOnly), true);
    ASSERT_EQ(qZipFile->readAll(), "Hello World!!!");
    qZipFile->close();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
