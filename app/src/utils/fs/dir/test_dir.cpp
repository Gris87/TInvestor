#include "src/utils/fs/dir/dir.h"

#include <QCoreApplication>
#include <gtest/gtest.h>



TEST(Test_Dir, Test_constructor_and_destructor)
{
    const Dir dir;
}

TEST(Test_Dir, Test_mkpath)
{
    Dir dir;

    const QString appDir = qApp->applicationDirPath();

    QDir qDir(appDir + "/test/some_dir");
    ASSERT_TRUE(!qDir.exists());

    ASSERT_TRUE(dir.mkpath(appDir + "/test/some_dir"));
    ASSERT_TRUE(qDir.exists());

    ASSERT_TRUE(qDir.removeRecursively());
}
