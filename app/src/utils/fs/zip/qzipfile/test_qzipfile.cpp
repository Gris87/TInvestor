#include "src/utils/fs/zip/qzipfile/qzipfile.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>



class Test_QZipFile : public ::testing::Test
{
protected:
    void SetUp()
    {
        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_qzipfile").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_qzipfile");

        zip = new QuaZip(appDir + "/test/dir_for_qzipfile/test.zip");
        zip->open(QuaZip::mdCreate);

        QuaZipFile zipFile(zip);

        QuaZipNewInfo info("hello.txt", "hello.txt");
        zipFile.open(QIODevice::WriteOnly, info);
        zipFile.write("AzazAZazzzaaz");
        zipFile.close();

        QuaZipNewInfo info2("world.txt", "world.txt");
        zipFile.open(QIODevice::WriteOnly, info2);
        zipFile.write("Hello World!!!");
        zipFile.close();

        zip->close();
        zip->open(QuaZip::mdUnzip);

        qZipFile = new QZipFile(zip);
    }

    void TearDown()
    {
        zip->close();

        delete qZipFile;
        delete zip;

        QDir(appDir + "/test/dir_for_qzipfile").removeRecursively();
    }

    QString   appDir;
    QuaZip*   zip;
    QZipFile* qZipFile;
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
