#include "src/fs/dir/dir.h"

#include <QCoreApplication>
#include <gtest/gtest.h>



TEST(Test_Dir, Test_constructor_and_destructor)
{
    Dir dir;
}

TEST(Test_Dir, Test_mkpath)
{
    Dir dir;

    QString appDir = qApp->applicationDirPath();

    QDir qDir(appDir + "/test/some/path");
    ASSERT_TRUE(!qDir.exists());

    ASSERT_TRUE(dir.mkpath(appDir + "/test/some/path"));
    ASSERT_TRUE(qDir.exists());

    ASSERT_TRUE(qDir.removeRecursively());
}
