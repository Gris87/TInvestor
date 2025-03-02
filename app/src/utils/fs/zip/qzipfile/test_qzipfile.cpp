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
    }

    void TearDown()
    {
        QDir(appDir + "/test/dir_for_qzipfile").removeRecursively();
    }

    QString appDir;
};



TEST_F(Test_QZipFile, Test_constructor_and_destructor)
{
}
