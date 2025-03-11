#include "src/utils/fs/zip/qzip/qzip.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/fs/zip/qzipfile/iqzipfile.h"



class Test_QZip : public ::testing::Test
{
protected:
    void SetUp()
    {
        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_qzip").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_qzip");

        QuaZip quaZip(appDir + "/test/dir_for_qzip/test.zip");
        quaZip.open(QuaZip::mdCreate);

        QuaZipFile zipFile(&quaZip);

        QuaZipNewInfo info("hello.txt", "hello.txt");
        zipFile.open(QIODevice::WriteOnly, info);
        zipFile.write("AzazAZazzzaaz");
        zipFile.close();

        QuaZipNewInfo info2("world.txt", "world.txt");
        zipFile.open(QIODevice::WriteOnly, info2);
        zipFile.write("Hello World!!!");
        zipFile.close();

        quaZip.close();

        qZip = new QZip(appDir + "/test/dir_for_qzip/test.zip");
    }

    void TearDown()
    {
        delete qZip;

        QDir(appDir + "/test/dir_for_qzip").removeRecursively();
    }

    QString appDir;
    QZip*   qZip;
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
