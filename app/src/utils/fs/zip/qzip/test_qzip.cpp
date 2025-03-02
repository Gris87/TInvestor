#include "src/utils/fs/zip/qzip/qzip.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>



class Test_QZip : public ::testing::Test
{
protected:
    void SetUp()
    {
        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_qzip").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_qzip");
    }

    void TearDown()
    {
        QDir(appDir + "/test/dir_for_qzip").removeRecursively();
    }

    QString appDir;
};



TEST_F(Test_QZip, Test_constructor_and_destructor)
{
}
