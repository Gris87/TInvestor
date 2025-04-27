#include "src/utils/fs/file/file.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
class Test_File : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_file").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_file");

        file  = new File(appDir + "/test/dir_for_file/test.txt");
        qFile = new QFile(appDir + "/test/dir_for_file/test.txt");
    }

    void TearDown() override
    {
        delete qFile;
        delete file;

        QDir(appDir + "/test/dir_for_file").removeRecursively();
    }

    File*   file;
    QFile*  qFile;
    QString appDir;
};



TEST_F(Test_File, Test_constructor_and_destructor)
{
}

TEST_F(Test_File, Test_write)
{
    const char* const bytes = "BLAH";

    ASSERT_TRUE(file->open(QIODevice::WriteOnly));
    ASSERT_EQ(file->write(bytes, strlen(bytes)), 4);
    file->close();

    ASSERT_EQ(file->size(), 4);

    ASSERT_TRUE(qFile->open(QIODevice::ReadOnly));
    QByteArray writtenData = qFile->readAll();
    qFile->close();

    ASSERT_EQ(writtenData.size(), 4);
    ASSERT_EQ(writtenData.at(0), 'B');
    ASSERT_EQ(writtenData.at(1), 'L');
    ASSERT_EQ(writtenData.at(2), 'A');
    ASSERT_EQ(writtenData.at(3), 'H');

    QByteArray data;
    data.append(1);
    data.append(2);
    data.append(3);

    ASSERT_TRUE(file->open(QIODevice::WriteOnly));
    ASSERT_EQ(file->write(data), 3);
    file->close();

    ASSERT_EQ(file->size(), 3);

    ASSERT_TRUE(qFile->open(QIODevice::ReadOnly));
    writtenData = qFile->readAll();
    qFile->close();

    ASSERT_EQ(writtenData.size(), 3);
    ASSERT_EQ(writtenData.at(0), 1);
    ASSERT_EQ(writtenData.at(1), 2);
    ASSERT_EQ(writtenData.at(2), 3);
}

TEST_F(Test_File, Test_append)
{
    ASSERT_TRUE(qFile->open(QIODevice::WriteOnly));
    ASSERT_EQ(qFile->write("TEST"), 4);
    qFile->close();

    ASSERT_EQ(file->size(), 4);

    const char* const bytes = "BLAH";

    ASSERT_TRUE(file->open(QIODevice::Append));
    ASSERT_EQ(file->write(bytes, strlen(bytes)), static_cast<qint64>(strlen(bytes)));
    file->close();

    ASSERT_EQ(file->size(), 8);

    ASSERT_TRUE(qFile->open(QIODevice::ReadOnly));
    QByteArray writtenData = qFile->readAll();
    qFile->close();

    ASSERT_EQ(writtenData.size(), 8);
    ASSERT_EQ(writtenData.at(0), 'T');
    ASSERT_EQ(writtenData.at(1), 'E');
    ASSERT_EQ(writtenData.at(2), 'S');
    ASSERT_EQ(writtenData.at(3), 'T');
    ASSERT_EQ(writtenData.at(4), 'B');
    ASSERT_EQ(writtenData.at(5), 'L');
    ASSERT_EQ(writtenData.at(6), 'A');
    ASSERT_EQ(writtenData.at(7), 'H');

    QByteArray data;
    data.append(1);
    data.append(2);
    data.append(3);

    ASSERT_TRUE(file->open(QIODevice::Append));
    ASSERT_EQ(file->write(data), 3);
    file->close();

    ASSERT_EQ(file->size(), 11);

    ASSERT_TRUE(qFile->open(QIODevice::ReadOnly));
    writtenData = qFile->readAll();
    qFile->close();

    ASSERT_EQ(writtenData.size(), 11);
    ASSERT_EQ(writtenData.at(0), 'T');
    ASSERT_EQ(writtenData.at(1), 'E');
    ASSERT_EQ(writtenData.at(2), 'S');
    ASSERT_EQ(writtenData.at(3), 'T');
    ASSERT_EQ(writtenData.at(4), 'B');
    ASSERT_EQ(writtenData.at(5), 'L');
    ASSERT_EQ(writtenData.at(6), 'A');
    ASSERT_EQ(writtenData.at(7), 'H');
    ASSERT_EQ(writtenData.at(8), 1);
    ASSERT_EQ(writtenData.at(9), 2);
    ASSERT_EQ(writtenData.at(10), 3);
}

TEST_F(Test_File, Test_read)
{
    ASSERT_TRUE(qFile->open(QIODevice::WriteOnly));
    ASSERT_EQ(qFile->write("TEST"), 4);
    qFile->close();

    ASSERT_EQ(file->size(), 4);

    ASSERT_TRUE(file->open(QIODevice::ReadOnly));
    QByteArray readData = file->readAll();
    file->close();

    ASSERT_EQ(readData.size(), 4);
    ASSERT_EQ(readData.at(0), 'T');
    ASSERT_EQ(readData.at(1), 'E');
    ASSERT_EQ(readData.at(2), 'S');
    ASSERT_EQ(readData.at(3), 'T');

    ASSERT_TRUE(qFile->open(QIODevice::WriteOnly));
    ASSERT_EQ(qFile->write("Hello"), 5);
    qFile->close();

    ASSERT_EQ(file->size(), 5);

    ASSERT_TRUE(file->open(QIODevice::ReadOnly));
    file->read(readData.data(), 2);
    file->close();

    ASSERT_EQ(readData.size(), 4);
    ASSERT_EQ(readData.at(0), 'H');
    ASSERT_EQ(readData.at(1), 'e');
    ASSERT_EQ(readData.at(2), 'S');
    ASSERT_EQ(readData.at(3), 'T');
}

TEST_F(Test_File, Test_exists)
{
    ASSERT_EQ(file->exists(), false);

    ASSERT_TRUE(qFile->open(QIODevice::WriteOnly));
    ASSERT_EQ(qFile->write("TEST"), 4);
    qFile->close();

    ASSERT_EQ(file->exists(), true);
}

TEST_F(Test_File, Test_remove)
{
    ASSERT_EQ(file->exists(), false);
    ASSERT_EQ(file->remove(), false);

    ASSERT_TRUE(qFile->open(QIODevice::WriteOnly));
    ASSERT_EQ(qFile->write("TEST"), 4);
    qFile->close();

    ASSERT_EQ(file->exists(), true);

    ASSERT_EQ(file->remove(), true);
    ASSERT_EQ(file->exists(), false);
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-function-cognitive-complexity)
