#include "src/db/instruments/instrumentsdatabase.h"

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



class Test_InstrumentsDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        appDir = qApp->applicationDirPath();

        StrictMock<DirMock>* dirMock = new StrictMock<DirMock>(); // Will be deleted in InstrumentsDatabase constructor
        dirFactoryMock               = new StrictMock<DirFactoryMock>();
        fileFactoryMock              = new StrictMock<FileFactoryMock>();

        EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock)));
        EXPECT_CALL(*dirMock, mkpath(appDir + "/data/instruments/logos")).WillOnce(Return(true));

        database = new InstrumentsDatabase(dirFactoryMock, fileFactoryMock);

        fillWithData();
    }

    void fillWithData()
    {
        const QString instrumentsStr =
            "{"
            "    \"aaaaa\": {\"ticker\": \"BANA\", \"name\": \"Banana\",       \"pricePrecision\": 2},"
            "    \"bbbbb\": {\"ticker\": \"BODO\", \"name\": \"BODO NE BODO\", \"pricePrecision\": 3},"
            "    \"ccccc\": {\"ticker\": \"ZARA\", \"name\": \"Zaraza\",       \"pricePrecision\": 4}"
            "}";

        testInstruments = instrumentsStr.toUtf8();
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
    }

    InstrumentsDatabase*         database;
    StrictMock<DirFactoryMock>*  dirFactoryMock;
    StrictMock<FileFactoryMock>* fileFactoryMock;
    QString                      appDir;
    QByteArray                   testInstruments;
};



TEST_F(Test_InstrumentsDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_InstrumentsDatabase, Test_readInstruments)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in readInstruments

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/instruments/instruments.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, readAll()).WillOnce(Return(testInstruments));
    EXPECT_CALL(*fileMock, close());

    const Instruments instruments = database->readInstruments();

    // clang-format off
    ASSERT_EQ(instruments.size(),                  3);
    ASSERT_EQ(instruments["aaaaa"].ticker,         "BANA");
    ASSERT_EQ(instruments["aaaaa"].name,           "Banana");
    ASSERT_EQ(instruments["aaaaa"].pricePrecision, 2);
    ASSERT_EQ(instruments["bbbbb"].ticker,         "BODO");
    ASSERT_EQ(instruments["bbbbb"].name,           "BODO NE BODO");
    ASSERT_EQ(instruments["bbbbb"].pricePrecision, 3);
    ASSERT_EQ(instruments["ccccc"].ticker,         "ZARA");
    ASSERT_EQ(instruments["ccccc"].name,           "Zaraza");
    ASSERT_EQ(instruments["ccccc"].pricePrecision, 4);
    // clang-format on
}

TEST_F(Test_InstrumentsDatabase, Test_writeInstruments)
{
    const InSequence seq;

    Instruments instruments;

    Instrument instrument1;
    Instrument instrument2;
    Instrument instrument3;

    instrument1.ticker         = "BANA";
    instrument1.name           = "Banana";
    instrument1.pricePrecision = 2;

    instrument2.ticker         = "BODO";
    instrument2.name           = "BODO NE BODO";
    instrument2.pricePrecision = 3;

    instrument3.ticker         = "ZARA";
    instrument3.name           = "Zaraza";
    instrument3.pricePrecision = 4;

    instruments["aaaaa"] = instrument1;
    instruments["bbbbb"] = instrument2;
    instruments["ccccc"] = instrument3;

    const QString instrumentsStr =
        "{\"aaaaa\":{\"name\":\"Banana\",\"pricePrecision\":2,\"ticker\":\"BANA\"},\"bbbbb\":{\"name\":\"BODO NE "
        "BODO\",\"pricePrecision\":3,\"ticker\":\"BODO\"},\"ccccc\":{\"name\":\"Zaraza\",\"pricePrecision\":4,\"ticker\":"
        "\"ZARA\"}}";
    const QByteArray instrumentsBytes = instrumentsStr.toUtf8();

    StrictMock<FileMock>* fileMock = new StrictMock<FileMock>(); // Will be deleted in writeInstruments

    EXPECT_CALL(*fileFactoryMock, newInstance(QString(appDir + "/data/instruments/instruments.json")))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock)));

    EXPECT_CALL(*fileMock, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock, write(instrumentsBytes)).WillOnce(Return(instrumentsBytes.size()));
    EXPECT_CALL(*fileMock, close());

    database->writeInstruments(instruments);
}
