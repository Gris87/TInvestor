#include "src/utils/fs/dir/dir.h"

#include <QCoreApplication>
#include <gtest/gtest.h>



TEST(Test_Dir, Test_constructor_and_destructor)
{
    const Dir dir;
}

TEST(Test_Dir, Test_mkpath_and_entryList_and_removeRecursively)
{
    Dir dir;

    const QString appDir = qApp->applicationDirPath();

    QDir qDir(appDir + "/test/some_dir");
    ASSERT_TRUE(!qDir.exists());

    ASSERT_TRUE(dir.mkpath(appDir + "/test/some_dir"));
    ASSERT_TRUE(qDir.exists());

    QFile qFile1(appDir + "/test/some_dir/a.png");
    QFile qFile2(appDir + "/test/some_dir/b.png");
    QFile qFile3(appDir + "/test/some_dir/c.ico");

    ASSERT_TRUE(qFile1.open(QIODevice::WriteOnly));
    ASSERT_TRUE(qFile2.open(QIODevice::WriteOnly));
    ASSERT_TRUE(qFile3.open(QIODevice::WriteOnly));
    qFile1.close();
    qFile2.close();
    qFile3.close();

    QStringList files = Dir(appDir + "/test/some_dir").entryList(QStringList() << "*.png");

    ASSERT_EQ(files.size(), 2);
    ASSERT_EQ(files.at(0), "a.png");
    ASSERT_EQ(files.at(1), "b.png");

    ASSERT_TRUE(Dir(appDir + "/test/some_dir").removeRecursively());
    ASSERT_TRUE(!qDir.exists());
}
