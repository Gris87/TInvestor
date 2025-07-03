#include "src/db/operations/operationsdatabase.h"

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



class Test_OperationsDatabase : public ::testing::Test
{
protected:
    void SetUp() override
    {
        appDir = qApp->applicationDirPath();

        dirFactoryMock   = new StrictMock<DirFactoryMock>();
        fileFactoryMock  = new StrictMock<FileFactoryMock>();
        logosStorageMock = new StrictMock<LogosStorageMock>();

        database = new OperationsDatabase(dirFactoryMock, fileFactoryMock, logosStorageMock, true);
    }

    void TearDown() override
    {
        delete database;
        delete dirFactoryMock;
        delete fileFactoryMock;
        delete logosStorageMock;
    }

    OperationsDatabase*           database;
    StrictMock<DirFactoryMock>*   dirFactoryMock;
    StrictMock<FileFactoryMock>*  fileFactoryMock;
    StrictMock<LogosStorageMock>* logosStorageMock;
    QString                       appDir;
};



TEST_F(Test_OperationsDatabase, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsDatabase, Test_readOperations)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in readOperations function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in readOperations function
    StrictMock<FileMock>* fileMock3 = new StrictMock<FileMock>(); // Will be deleted in readOperations function
    StrictMock<FileMock>* fileMock4 = new StrictMock<FileMock>(); // Will be deleted in readOperations function

    QList<Operation> operations;
    Logo             logo;

    QString operationsStr =
        "{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{"
        "\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{"
        "\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\","
        "\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,"
        "\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},"
        "\"remainedQuantity\":6,\"timestamp\":1000,\"totalMoney\":{\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{"
        "\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,\"yieldWithCommission\":15,"
        "\"yieldWithCommissionPercent\":16},\n{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,"
        "\"commissionPrecision\":30,\"costFifo\":{\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},"
        "\"description\":\"d\",\"fifoItems\":[{\"cost\":{\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,"
        "\"units\":17},\"instrumentId\":\"aaaaa\",\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{"
        "\"nano\":20,"
        "\"units\":19},\"payment\":7,\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{"
        "\"nano\":25,\"units\":24},\"remainedQuantity\":6,\"timestamp\":2000,\"totalMoney\":{\"nano\":27,\"units\":26},"
        "\"totalYieldWithCommission\":{\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,"
        "\"yieldWithCommission\":15,\"yieldWithCommissionPercent\":16},\n{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":"
        "4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,"
        "\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{\"nano\":33,\"units\":32},\"quantity\":31}],"
        "\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\",\"instrumentName\":\"c\",\"instrumentTicker\":"
        "\"b\","
        "\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,"
        "\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},\"remainedQuantity\":6,\"timestamp\":3000,\"totalMoney\":{"
        "\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":"
        "23,\"yield\":14,\"yieldWithCommission\":15,\"yieldWithCommissionPercent\":16}";
    QByteArray operationsBytes = operationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, readAll()).WillOnce(Return(operationsBytes));
    EXPECT_CALL(*fileMock1, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).Times(3).WillRepeatedly(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    database->setAutoPilotMode(true);
    database->setAccount("ACCOUNT_ID");

    operations = database->readOperations();

    ASSERT_EQ(operations.size(), 3);

    for (int i = 0; i < operations.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(operations.at(i).timestamp,                         3000 - i * 1000);
        ASSERT_EQ(operations.at(i).instrumentId,                      "aaaaa");
        ASSERT_EQ(operations.at(i).instrumentLogo,                    &logo);
        ASSERT_EQ(operations.at(i).instrumentTicker,                  "b");
        ASSERT_EQ(operations.at(i).instrumentName,                    "c");
        ASSERT_EQ(operations.at(i).description,                       "d");
        ASSERT_NEAR(operations.at(i).price,                           2, 0.0001f);
        ASSERT_EQ(operations.at(i).fifoItems.size(),                  1);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).quantity,          31);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).cost.units,        32);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).cost.nano,         33);
        ASSERT_NEAR(operations.at(i).avgPriceFifo,                    3, 0.0001f);
        ASSERT_NEAR(operations.at(i).avgPriceWavg,                    4, 0.0001f);
        ASSERT_EQ(operations.at(i).quantity,                          5);
        ASSERT_EQ(operations.at(i).remainedQuantity,                  6);
        ASSERT_NEAR(operations.at(i).payment,                         7, 0.0001f);
        ASSERT_NEAR(operations.at(i).avgCostFifo,                     8, 0.0001f);
        ASSERT_EQ(operations.at(i).costFifo.units,                    9);
        ASSERT_EQ(operations.at(i).costFifo.nano,                     10);
        ASSERT_EQ(operations.at(i).costWavg.units,                    11);
        ASSERT_EQ(operations.at(i).costWavg.nano,                     12);
        ASSERT_NEAR(operations.at(i).commission,                      13, 0.0001f);
        ASSERT_NEAR(operations.at(i).yield,                           14, 0.0001f);
        ASSERT_NEAR(operations.at(i).yieldWithCommission,             15, 0.0001f);
        ASSERT_NEAR(operations.at(i).yieldWithCommissionPercent,      16, 0.0001f);
        ASSERT_EQ(operations.at(i).inputMoney.units,                  17);
        ASSERT_EQ(operations.at(i).inputMoney.nano,                   18);
        ASSERT_EQ(operations.at(i).maxInputMoney.units,               19);
        ASSERT_EQ(operations.at(i).maxInputMoney.nano,                20);
        ASSERT_EQ(operations.at(i).totalYieldWithCommission.units,    21);
        ASSERT_EQ(operations.at(i).totalYieldWithCommission.nano,     22);
        ASSERT_NEAR(operations.at(i).totalYieldWithCommissionPercent, 23, 0.0001f);
        ASSERT_EQ(operations.at(i).remainedMoney.units,               24);
        ASSERT_EQ(operations.at(i).remainedMoney.nano,                25);
        ASSERT_EQ(operations.at(i).totalMoney.units,                  26);
        ASSERT_EQ(operations.at(i).totalMoney.nano,                   27);
        ASSERT_EQ(operations.at(i).pricePrecision,                    28);
        ASSERT_EQ(operations.at(i).paymentPrecision,                  29);
        ASSERT_EQ(operations.at(i).commissionPrecision,               30);
        // clang-format on
    }

    QString largeOperationsStr;

    for (int i = 0; i < 1000; ++i)
    {
        if (i > 0)
        {
            largeOperationsStr += ",\n";
        }

        largeOperationsStr +=
            QString(
                "{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,\"commissionPrecision\":30,"
                "\"costFifo\":{\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},\"description\":\"d\","
                "\"fifoItems\":[{\"cost\":{\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,\"units\":17},"
                "\"instrumentId\":\"aaaaa\",\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{\"nano\":20,"
                "\"units\":19},\"payment\":7,\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,"
                "\"remainedMoney\":{\"nano\":25,\"units\":24},\"remainedQuantity\":6,\"timestamp\":%1,\"totalMoney\":{\"nano\":"
                "27,\"units\":26},\"totalYieldWithCommission\":{\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,"
                "\"yield\":14,\"yieldWithCommission\":15,\"yieldWithCommissionPercent\":16}"
            )
                .arg(i * 1000);
    }

    QByteArray largeOperationsBytes = largeOperationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, readAll()).WillOnce(Return(largeOperationsBytes));
    EXPECT_CALL(*fileMock2, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).Times(1000).WillRepeatedly(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    operations = database->readOperations();

    ASSERT_EQ(operations.size(), 1000);

    for (int i = 0; i < operations.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(operations.at(i).timestamp,                         999000 - i * 1000);
        ASSERT_EQ(operations.at(i).instrumentId,                      "aaaaa");
        ASSERT_EQ(operations.at(i).instrumentLogo,                    &logo);
        ASSERT_EQ(operations.at(i).instrumentTicker,                  "b");
        ASSERT_EQ(operations.at(i).instrumentName,                    "c");
        ASSERT_EQ(operations.at(i).description,                       "d");
        ASSERT_NEAR(operations.at(i).price,                           2, 0.0001f);
        ASSERT_EQ(operations.at(i).fifoItems.size(),                  1);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).quantity,          31);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).cost.units,        32);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).cost.nano,         33);
        ASSERT_NEAR(operations.at(i).avgPriceFifo,                    3, 0.0001f);
        ASSERT_NEAR(operations.at(i).avgPriceWavg,                    4, 0.0001f);
        ASSERT_EQ(operations.at(i).quantity,                          5);
        ASSERT_EQ(operations.at(i).remainedQuantity,                  6);
        ASSERT_NEAR(operations.at(i).payment,                         7, 0.0001f);
        ASSERT_NEAR(operations.at(i).avgCostFifo,                     8, 0.0001f);
        ASSERT_EQ(operations.at(i).costFifo.units,                    9);
        ASSERT_EQ(operations.at(i).costFifo.nano,                     10);
        ASSERT_EQ(operations.at(i).costWavg.units,                    11);
        ASSERT_EQ(operations.at(i).costWavg.nano,                     12);
        ASSERT_NEAR(operations.at(i).commission,                      13, 0.0001f);
        ASSERT_NEAR(operations.at(i).yield,                           14, 0.0001f);
        ASSERT_NEAR(operations.at(i).yieldWithCommission,             15, 0.0001f);
        ASSERT_NEAR(operations.at(i).yieldWithCommissionPercent,      16, 0.0001f);
        ASSERT_EQ(operations.at(i).inputMoney.units,                  17);
        ASSERT_EQ(operations.at(i).inputMoney.nano,                   18);
        ASSERT_EQ(operations.at(i).maxInputMoney.units,               19);
        ASSERT_EQ(operations.at(i).maxInputMoney.nano,                20);
        ASSERT_EQ(operations.at(i).totalYieldWithCommission.units,    21);
        ASSERT_EQ(operations.at(i).totalYieldWithCommission.nano,     22);
        ASSERT_NEAR(operations.at(i).totalYieldWithCommissionPercent, 23, 0.0001f);
        ASSERT_EQ(operations.at(i).remainedMoney.units,               24);
        ASSERT_EQ(operations.at(i).remainedMoney.nano,                25);
        ASSERT_EQ(operations.at(i).totalMoney.units,                  26);
        ASSERT_EQ(operations.at(i).totalMoney.nano,                   27);
        ASSERT_EQ(operations.at(i).pricePrecision,                    28);
        ASSERT_EQ(operations.at(i).paymentPrecision,                  29);
        ASSERT_EQ(operations.at(i).commissionPrecision,               30);
        // clang-format on
    }

    operationsStr   += ",\n{Bad content ::::: 555";
    operationsBytes  = operationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock3)));
    EXPECT_CALL(*fileMock3, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock3, readAll()).WillOnce(Return(operationsBytes));
    EXPECT_CALL(*fileMock3, close());

    database->setAutoPilotMode(false);

    operations = database->readOperations();

    ASSERT_EQ(operations.size(), 0);

    largeOperationsStr   += ",\n{Bad content ::::: 555";
    largeOperationsBytes  = largeOperationsStr.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock4)));
    EXPECT_CALL(*fileMock4, open(QIODevice::OpenMode(QIODevice::ReadOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock4, readAll()).WillOnce(Return(largeOperationsBytes));
    EXPECT_CALL(*fileMock4, close());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).Times(1000).WillRepeatedly(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());

    operations = database->readOperations();

    ASSERT_EQ(operations.size(), 1001);

    for (int i = 1; i < operations.size(); ++i)
    {
        // clang-format off
        ASSERT_EQ(operations.at(i).timestamp,                         1000000 - i * 1000);
        ASSERT_EQ(operations.at(i).instrumentId,                      "aaaaa");
        ASSERT_EQ(operations.at(i).instrumentLogo,                    &logo);
        ASSERT_EQ(operations.at(i).instrumentTicker,                  "b");
        ASSERT_EQ(operations.at(i).instrumentName,                    "c");
        ASSERT_EQ(operations.at(i).description,                       "d");
        ASSERT_NEAR(operations.at(i).price,                           2, 0.0001f);
        ASSERT_EQ(operations.at(i).fifoItems.size(),                  1);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).quantity,          31);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).cost.units,        32);
        ASSERT_EQ(operations.at(i).fifoItems.at(0).cost.nano,         33);
        ASSERT_NEAR(operations.at(i).avgPriceFifo,                    3, 0.0001f);
        ASSERT_NEAR(operations.at(i).avgPriceWavg,                    4, 0.0001f);
        ASSERT_EQ(operations.at(i).quantity,                          5);
        ASSERT_EQ(operations.at(i).remainedQuantity,                  6);
        ASSERT_NEAR(operations.at(i).payment,                         7, 0.0001f);
        ASSERT_NEAR(operations.at(i).avgCostFifo,                     8, 0.0001f);
        ASSERT_EQ(operations.at(i).costFifo.units,                    9);
        ASSERT_EQ(operations.at(i).costFifo.nano,                     10);
        ASSERT_EQ(operations.at(i).costWavg.units,                    11);
        ASSERT_EQ(operations.at(i).costWavg.nano,                     12);
        ASSERT_NEAR(operations.at(i).commission,                      13, 0.0001f);
        ASSERT_NEAR(operations.at(i).yield,                           14, 0.0001f);
        ASSERT_NEAR(operations.at(i).yieldWithCommission,             15, 0.0001f);
        ASSERT_NEAR(operations.at(i).yieldWithCommissionPercent,      16, 0.0001f);
        ASSERT_EQ(operations.at(i).inputMoney.units,                  17);
        ASSERT_EQ(operations.at(i).inputMoney.nano,                   18);
        ASSERT_EQ(operations.at(i).maxInputMoney.units,               19);
        ASSERT_EQ(operations.at(i).maxInputMoney.nano,                20);
        ASSERT_EQ(operations.at(i).totalYieldWithCommission.units,    21);
        ASSERT_EQ(operations.at(i).totalYieldWithCommission.nano,     22);
        ASSERT_NEAR(operations.at(i).totalYieldWithCommissionPercent, 23, 0.0001f);
        ASSERT_EQ(operations.at(i).remainedMoney.units,               24);
        ASSERT_EQ(operations.at(i).remainedMoney.nano,                25);
        ASSERT_EQ(operations.at(i).totalMoney.units,                  26);
        ASSERT_EQ(operations.at(i).totalMoney.nano,                   27);
        ASSERT_EQ(operations.at(i).pricePrecision,                    28);
        ASSERT_EQ(operations.at(i).paymentPrecision,                  29);
        ASSERT_EQ(operations.at(i).commissionPrecision,               30);
        // clang-format on
    }
}

TEST_F(Test_OperationsDatabase, Test_writeOperations)
{
    const InSequence seq;

    StrictMock<DirMock>*  dirMock1  = new StrictMock<DirMock>();  // Will be deleted in writeOperations function
    StrictMock<DirMock>*  dirMock2  = new StrictMock<DirMock>();  // Will be deleted in writeOperations function
    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in writeOperations function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in writeOperations function

    QList<Operation> operations;
    Operation        operation;

    operation.timestamp                       = 3000;
    operation.instrumentId                    = "aaaaa";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCostFifo                     = 8.0f;
    operation.costFifo.units                  = 9;
    operation.costFifo.nano                   = 10;
    operation.costWavg.units                  = 11;
    operation.costWavg.nano                   = 12;
    operation.commission                      = 13.0f;
    operation.yield                           = 14.0f;
    operation.yieldWithCommission             = 15.0f;
    operation.yieldWithCommissionPercent      = 16.0f;
    operation.inputMoney.units                = 17;
    operation.inputMoney.nano                 = 18;
    operation.maxInputMoney.units             = 19;
    operation.maxInputMoney.nano              = 20;
    operation.totalYieldWithCommission.units  = 21;
    operation.totalYieldWithCommission.nano   = 22;
    operation.totalYieldWithCommissionPercent = 23;
    operation.remainedMoney.units             = 24;
    operation.remainedMoney.nano              = 25;
    operation.totalMoney.units                = 26;
    operation.totalMoney.nano                 = 27;
    operation.pricePrecision                  = 28;
    operation.paymentPrecision                = 29;
    operation.commissionPrecision             = 30;

    OperationFifoItem item;

    item.quantity   = 31;
    item.cost.units = 32;
    item.cost.nano  = 33;

    operation.fifoItems.append(item);
    operations.append(operation);

    operation.timestamp = 2000;
    operations.append(operation);

    operation.timestamp = 1000;
    operations.append(operation);

    const QString operationsStr =
        "{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{"
        "\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{"
        "\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\","
        "\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,"
        "\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},"
        "\"remainedQuantity\":6,\"timestamp\":1000,\"totalMoney\":{\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{"
        "\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,\"yieldWithCommission\":15,"
        "\"yieldWithCommissionPercent\":16},\n{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,"
        "\"commissionPrecision\":30,\"costFifo\":{\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},"
        "\"description\":\"d\",\"fifoItems\":[{\"cost\":{\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,"
        "\"units\":17},\"instrumentId\":\"aaaaa\",\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{"
        "\"nano\":20,"
        "\"units\":19},\"payment\":7,\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{"
        "\"nano\":25,\"units\":24},\"remainedQuantity\":6,\"timestamp\":2000,\"totalMoney\":{\"nano\":27,\"units\":26},"
        "\"totalYieldWithCommission\":{\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,"
        "\"yieldWithCommission\":15,\"yieldWithCommissionPercent\":16},\n{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":"
        "4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,"
        "\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{\"nano\":33,\"units\":32},\"quantity\":31}],"
        "\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\",\"instrumentName\":\"c\",\"instrumentTicker\":"
        "\"b\","
        "\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,"
        "\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},\"remainedQuantity\":6,\"timestamp\":3000,\"totalMoney\":{"
        "\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":"
        "23,\"yield\":14,\"yieldWithCommission\":15,\"yieldWithCommissionPercent\":16}";
    const QByteArray operationsBytes = operationsStr.toUtf8();

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock1)));
    EXPECT_CALL(*dirMock1, mkpath(appDir + "/data/autopilot/ACCOUNT_ID")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, write(operationsBytes)).WillOnce(Return(operationsBytes.size()));
    EXPECT_CALL(*fileMock1, close());

    database->setAutoPilotMode(true);
    database->setAccount("ACCOUNT_ID");

    database->writeOperations(operations);

    EXPECT_CALL(*dirFactoryMock, newInstance(QString())).WillOnce(Return(std::shared_ptr<IDir>(dirMock2)));
    EXPECT_CALL(*dirMock2, mkpath(appDir + "/data/simulator")).WillOnce(Return(true));
    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::WriteOnly))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, write(operationsBytes)).WillOnce(Return(operationsBytes.size()));
    EXPECT_CALL(*fileMock2, close());

    database->setAutoPilotMode(false);

    database->writeOperations(operations);
}

TEST_F(Test_OperationsDatabase, Test_appendOperations)
{
    const InSequence seq;

    StrictMock<FileMock>* fileMock1 = new StrictMock<FileMock>(); // Will be deleted in appendOperations function
    StrictMock<FileMock>* fileMock2 = new StrictMock<FileMock>(); // Will be deleted in appendOperations function

    QList<Operation> operations;
    Operation        operation;

    operation.timestamp                       = 6000;
    operation.instrumentId                    = "aaaaa";
    operation.instrumentTicker                = "b";
    operation.instrumentName                  = "c";
    operation.description                     = "d";
    operation.price                           = 2.0f;
    operation.avgPriceFifo                    = 3.0f;
    operation.avgPriceWavg                    = 4.0f;
    operation.quantity                        = 5;
    operation.remainedQuantity                = 6;
    operation.payment                         = 7.0f;
    operation.avgCostFifo                     = 8.0f;
    operation.costFifo.units                  = 9;
    operation.costFifo.nano                   = 10;
    operation.costWavg.units                  = 11;
    operation.costWavg.nano                   = 12;
    operation.commission                      = 13.0f;
    operation.yield                           = 14.0f;
    operation.yieldWithCommission             = 15.0f;
    operation.yieldWithCommissionPercent      = 16.0f;
    operation.inputMoney.units                = 17;
    operation.inputMoney.nano                 = 18;
    operation.maxInputMoney.units             = 19;
    operation.maxInputMoney.nano              = 20;
    operation.totalYieldWithCommission.units  = 21;
    operation.totalYieldWithCommission.nano   = 22;
    operation.totalYieldWithCommissionPercent = 23;
    operation.remainedMoney.units             = 24;
    operation.remainedMoney.nano              = 25;
    operation.totalMoney.units                = 26;
    operation.totalMoney.nano                 = 27;
    operation.pricePrecision                  = 28;
    operation.paymentPrecision                = 29;
    operation.commissionPrecision             = 30;

    OperationFifoItem item;

    item.quantity   = 31;
    item.cost.units = 32;
    item.cost.nano  = 33;

    operation.fifoItems.append(item);
    operations.append(operation);

    operation.timestamp = 5000;
    operations.append(operation);

    operation.timestamp = 4000;
    operations.append(operation);

    const QString operation1Str =
        "{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{"
        "\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{"
        "\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\","
        "\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,"
        "\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},"
        "\"remainedQuantity\":6,\"timestamp\":4000,\"totalMoney\":{\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{"
        "\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,\"yieldWithCommission\":15,"
        "\"yieldWithCommissionPercent\":16}";
    const QString operation2Str =
        "{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{"
        "\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{"
        "\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\","
        "\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,"
        "\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},"
        "\"remainedQuantity\":6,\"timestamp\":5000,\"totalMoney\":{\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{"
        "\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,\"yieldWithCommission\":15,"
        "\"yieldWithCommissionPercent\":16}";
    const QString operation3Str =
        "{\"avgCostFifo\":8,\"avgPriceFifo\":3,\"avgPriceWavg\":4,\"commission\":13,\"commissionPrecision\":30,\"costFifo\":{"
        "\"nano\":10,\"units\":9},\"costWavg\":{\"nano\":12,\"units\":11},\"description\":\"d\",\"fifoItems\":[{\"cost\":{"
        "\"nano\":33,\"units\":32},\"quantity\":31}],\"inputMoney\":{\"nano\":18,\"units\":17},\"instrumentId\":\"aaaaa\","
        "\"instrumentName\":\"c\",\"instrumentTicker\":\"b\",\"maxInputMoney\":{\"nano\":20,\"units\":19},\"payment\":7,"
        "\"paymentPrecision\":29,\"price\":2,\"pricePrecision\":28,\"quantity\":5,\"remainedMoney\":{\"nano\":25,\"units\":24},"
        "\"remainedQuantity\":6,\"timestamp\":6000,\"totalMoney\":{\"nano\":27,\"units\":26},\"totalYieldWithCommission\":{"
        "\"nano\":22,\"units\":21},\"totalYieldWithCommissionPercent\":23,\"yield\":14,\"yieldWithCommission\":15,"
        "\"yieldWithCommissionPercent\":16}";
    const QByteArray operation1Bytes = operation1Str.toUtf8();
    const QByteArray operation2Bytes = operation2Str.toUtf8();
    const QByteArray operation3Bytes = operation3Str.toUtf8();

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/autopilot/ACCOUNT_ID/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock1)));
    EXPECT_CALL(*fileMock1, open(QIODevice::OpenMode(QIODevice::Append))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock1, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock1, write(operation1Bytes)).WillOnce(Return(operation1Bytes.size()));
    EXPECT_CALL(*fileMock1, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock1, write(operation2Bytes)).WillOnce(Return(operation2Bytes.size()));
    EXPECT_CALL(*fileMock1, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock1, write(operation3Bytes)).WillOnce(Return(operation3Bytes.size()));
    EXPECT_CALL(*fileMock1, close());

    database->setAutoPilotMode(true);
    database->setAccount("ACCOUNT_ID");

    database->appendOperations(operations);

    EXPECT_CALL(*fileFactoryMock, newInstance(appDir + "/data/simulator/operations.json"))
        .WillOnce(Return(std::shared_ptr<IFile>(fileMock2)));
    EXPECT_CALL(*fileMock2, open(QIODevice::OpenMode(QIODevice::Append))).WillOnce(Return(true));
    EXPECT_CALL(*fileMock2, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock2, write(operation1Bytes)).WillOnce(Return(operation1Bytes.size()));
    EXPECT_CALL(*fileMock2, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock2, write(operation2Bytes)).WillOnce(Return(operation2Bytes.size()));
    EXPECT_CALL(*fileMock2, write(QByteArray(",\n"))).WillOnce(Return(2));
    EXPECT_CALL(*fileMock2, write(operation3Bytes)).WillOnce(Return(operation3Bytes.size()));
    EXPECT_CALL(*fileMock2, close());

    database->setAutoPilotMode(false);

    database->appendOperations(operations);
}
