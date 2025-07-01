#include "src/db/logos/logosdatabase.h"

#include <QBuffer>
#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogosDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        dirFactoryMock  = new StrictMock<DirFactoryMock>();
        fileFactoryMock = new StrictMock<FileFactoryMock>();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in LogosDatabase constructor

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/instruments/logos")).WillOnce(Return(true));

        database = new LogosDatabase(dirFactoryMock, fileFactoryMock);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    LogosDatabase*               database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
    StrictMock<FileFactoryMock>* fileFactoryMock;
    QString                      appDir;
};



TEST_F(Test_LogosDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogosDatabase, Test_prepareLogos)
{
    const InSequence seq;

    StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in prepareLogos function

    QStringList files;
    files << "aaaaa.png";

    EXPECT_CALL(*dirFactoryMock, newInstance(appDir + "/data/instruments/logos/"))
        .WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
    EXPECT_CALL(
        *dirMock,
        entryList(
            QStringList() << "*.png", static_cast<QDir::Filters>(QDir::NoFilter), static_cast<QDir::SortFlags>(QDir::NoSort)
        )
    )
        .WillOnce(Return(files));

    const Logos logos = database->prepareLogos();

    // clang-format off
    ASSERT_EQ(logos.size(),            1);
    ASSERT_EQ(logos.contains("aaaaa"), true);
    // clang-format on
}

TEST_F(Test_LogosDatabase, Test_readLogo)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in readLogo function

    const QPixmap srcLogo(1, 1);
    QBuffer       buffer;

    srcLogo.save(&buffer, "PNG");

    QPixmap logo;

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/instruments/logos/aaaaa.png"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, readAll()).WillOnce(Return(buffer.buffer()));
    EXPECT_CALL(*fileMock, close());

    database->readLogo("aaaaa", &logo);

    ASSERT_EQ(logo.isNull(), false);
}

TEST_F(Test_LogosDatabase, Test_writeLogo)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writeLogo function

    QPixmap logo;
    QBuffer buffer;

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/instruments/logos/aaaaa.png"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));
    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, getDevice()).WillOnce(Return(&buffer));
    EXPECT_CALL(*fileMock, close());

    database->writeLogo("aaaaa", &logo);

    ASSERT_EQ(buffer.size(), 0);
}
