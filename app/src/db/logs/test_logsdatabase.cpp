#include "src/db/logs/logsdatabase.h"

#include <QCoreApplication>
#include <gtest/gtest.h>

#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/utils/fs/dir/idir_mock.h"
#include "src/utils/fs/dir/idirfactory_mock.h"
#include "src/utils/fs/file/ifile_mock.h"
#include "src/utils/fs/file/ifilefactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogsDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();

        dirFactoryMock   = new StrictMock<DirFactoryMock>();
        fileFactoryMock  = new StrictMock<FileFactoryMock>();
        logosStorageMock = new StrictMock<LogosStorageMock>();

        database = new LogsDatabase(dirFactoryMock, fileFactoryMock, logosStorageMock, true);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
        delete logosStorageMock;
    }

    LogsDatabase*                 database;
    StrictMock<DirFactoryMock>*   dirFactoryMock;
    StrictMock<FileFactoryMock>*  fileFactoryMock;
    StrictMock<LogosStorageMock>* logosStorageMock;
    QString                       appDir;
};



TEST_F(Test_LogsDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsDatabase, Test_readLogs)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readLogs function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readLogs function
    StrictMock<FileMock>* fileMock3 = new StrictMock<FileMock>(); // Will be deleted in readLogs function
    StrictMock<FileMock>* fileMock4 = new StrictMock<FileMock>(); // Will be deleted in readLogs function

    QList<LogEntry> entries;
    Logo            logo;

    QString entriesStr =
        "{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
        "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start "
        "annihilation\",\"timestamp\":1000},\n{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
        "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start "
        "annihilation\",\"timestamp\":2000},\n{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
        "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start annihilation\",\"timestamp\":3000}";
    QByteArray entriesBytes = entriesStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return(entriesBytes));
    EXPECT_CALL(*fileMock1, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).Times(3).WillRepeatedly(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    database->setAutoPilotMode(true);
    database->setAccount("ACCOUNT_ID");

    entries = database->readLogs();

    ASSERT_EQ(entries.size(), 3);

    for (int i = 0; i < entries.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(entries.at(i).timestamp,        3000 - i * 1000);
        ASSERT_EQ(entries.at(i).level,            LOG_LEVEL_WARNING);
        ASSERT_EQ(entries.at(i).instrumentId,     "aaaaa");
        ASSERT_EQ(entries.at(i).instrumentLogo,   &logo);
        ASSERT_EQ(entries.at(i).instrumentTicker, "VOVA");
        ASSERT_EQ(entries.at(i).instrumentName,   "Vovin papka");
        ASSERT_EQ(entries.at(i).message,          "Start annihilation");
        // clang-format on
    }

    QString largeEntriesStr;

    for (int i = 0; i < 1000; ++i)
    {
        if (i > 0)
        {
            largeEntriesStr += ",\n";
        }

        largeEntriesStr +=
            QString(
                "{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
                "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start annihilation\",\"timestamp\":%1}"
            )
                .arg(i * 1000);
    }

    QByteArray largeEntriesBytes = largeEntriesStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(largeEntriesBytes));
    EXPECT_CALL(*fileMock2, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).Times(1000).WillRepeatedly(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    entries = database->readLogs();

    ASSERT_EQ(entries.size(), 1000);

    for (int i = 0; i < entries.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(entries.at(i).timestamp,        999000 - i * 1000);
        ASSERT_EQ(entries.at(i).level,            LOG_LEVEL_WARNING);
        ASSERT_EQ(entries.at(i).instrumentId,     "aaaaa");
        ASSERT_EQ(entries.at(i).instrumentLogo,   &logo);
        ASSERT_EQ(entries.at(i).instrumentTicker, "VOVA");
        ASSERT_EQ(entries.at(i).instrumentName,   "Vovin papka");
        ASSERT_EQ(entries.at(i).message,          "Start annihilation");
        // clang-format on
    }

    entriesStr   += ",\n{Bad content ::::: 555";
    entriesBytes  = entriesStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock3)));
    EXPECT_CALL(*fileMock3, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock3, readAll()).WillOnce(Return(entriesBytes));
    EXPECT_CALL(*fileMock3, close());

    database->setAutoPilotMode(false);

    entries = database->readLogs();

    ASSERT_EQ(entries.size(), 0);

    largeEntriesStr   += ",\n{Bad content ::::: 555";
    largeEntriesBytes  = largeEntriesStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock4)));
    EXPECT_CALL(*fileMock4, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock4, readAll()).WillOnce(Return(largeEntriesBytes));
    EXPECT_CALL(*fileMock4, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).Times(1000).WillRepeatedly(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    entries = database->readLogs();

    ASSERT_EQ(entries.size(), 1001);

    for (int i = 1; i < entries.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(entries.at(i).timestamp,        1000000 - i * 1000);
        ASSERT_EQ(entries.at(i).level,            LOG_LEVEL_WARNING);
        ASSERT_EQ(entries.at(i).instrumentId,     "aaaaa");
        ASSERT_EQ(entries.at(i).instrumentLogo,   &logo);
        ASSERT_EQ(entries.at(i).instrumentTicker, "VOVA");
        ASSERT_EQ(entries.at(i).instrumentName,   "Vovin papka");
        ASSERT_EQ(entries.at(i).message,          "Start annihilation");
        // clang-format on
    }
}

TEST_F(Test_LogsDatabase, Test_writeLogs)
{
    const InSequence seq;

    StrictMock<DirMock>*  dirMock1  = new StrictMock<DirMock>();  // Will be deleted in writeLogs function
    StrictMock<DirMock>*  dirMock2  = new StrictMock<DirMock>();  // Will be deleted in writeLogs function
    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in writeLogs function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in writeLogs function

    QList<LogEntry> entries;
    LogEntry        entry;

    entry.timestamp        = 3000;
    entry.level            = LOG_LEVEL_WARNING;
    entry.instrumentId     = "aaaaa";
    entry.instrumentTicker = "VOVA";
    entry.instrumentName   = "Vovin papka";
    entry.message          = "Start annihilation";
    entries.append(entry);

    entry.timestamp = 2000;
    entries.append(entry);

    entry.timestamp = 1000;
    entries.append(entry);

    const QString entriesStr =
        "{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
        "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start "
        "annihilation\",\"timestamp\":1000},\n{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
        "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start "
        "annihilation\",\"timestamp\":2000},\n{\"instrumentId\":\"aaaaa\",\"instrumentName\":\"Vovin "
        "papka\",\"instrumentTicker\":\"VOVA\",\"level\":3,\"message\":\"Start annihilation\",\"timestamp\":3000}";
    const QByteArray entriesBytes = entriesStr.toUtf8();

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock1)));
    EXPECT_CALL(*dirMock1, mkpath(appDir + "/data/autopilot/ACCOUNT_ID")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, write(entriesBytes)).WillOnce(Return(entriesBytes.size()));
    EXPECT_CALL(*fileMock1, close());

    database->setAutoPilotMode(true);
    database->setAccount("ACCOUNT_ID");

    database->writeLogs(entries);

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock2)));
    EXPECT_CALL(*dirMock2, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, write(entriesBytes)).WillOnce(Return(entriesBytes.size()));
    EXPECT_CALL(*fileMock2, close());

    database->setAutoPilotMode(false);

    database->writeLogs(entries);
}

TEST_F(Test_LogsDatabase, Test_appendLog)
{
    const InSequence seq;

    StrictMock<DirMock>*  dirMock1  = new StrictMock<DirMock>();  // Will be deleted in appendLog function
    StrictMock<DirMock>*  dirMock2  = new StrictMock<DirMock>();  // Will be deleted in appendLog function
    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in appendLog function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in appendLog function

    LogEntry entry;

    entry.timestamp        = 4000;
    entry.level            = LOG_LEVEL_WARNING;
    entry.instrumentId     = "aaaaa";
    entry.instrumentTicker = "VOVA";
    entry.instrumentName   = "Vovin papka";
    entry.message          = "Start annihilation";

    const QString entryStr =
        R"({"instrumentId":"aaaaa","instrumentName":"Vovin papka","instrumentTicker":"VOVA","level":3,"message":"Start annihilation","timestamp":4000})";
    const QByteArray entryBytes = entryStr.toUtf8();

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock1)));
    EXPECT_CALL(*dirMock1, mkpath(appDir + "/data/autopilot/ACCOUNT_ID")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::WriteOnly | QIODevice::Append))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, size()).WillOnce(Return(1));
    EXPECT_CALL(*fileMock1, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock1, write(entryBytes)).WillOnce(Return(entryBytes.size()));
    EXPECT_CALL(*fileMock1, close());

    database->setAutoPilotMode(true);
    database->setAccount("ACCOUNT_ID");

    database->appendLog(entry);

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock2)));
    EXPECT_CALL(*dirMock2, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/logs.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::WriteOnly | QIODevice::Append))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, size()).WillOnce(Return(1));
    EXPECT_CALL(*fileMock2, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock2, write(entryBytes)).WillOnce(Return(entryBytes.size()));
    EXPECT_CALL(*fileMock2, close());

    database->setAutoPilotMode(false);

    database->appendLog(entry);
}
