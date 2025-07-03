#include "src/threads/operations/operationsthread.h"

#include <gtest/gtest.h>

#include "src/db/operations/ioperationsdatabase_mock.h"
#include "src/grpc/igrpcclient_mock.h"
#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"



const char* const RUBLE_UID = "a92e2e25-a698-45cc-a781-167cf465257c";



using ::testing::Gt;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_OperationsThread : public ::testing::Test
{
protected:
    void SetUp() override
    {
        operationsDatabaseMock = new StrictMock<OperationsDatabaseMock>();
        instrumentsStorageMock = new StrictMock<InstrumentsStorageMock>();
        logosStorageMock       = new StrictMock<LogosStorageMock>();
        grpcClientMock         = new StrictMock<GrpcClientMock>();

        thread = new OperationsThread(operationsDatabaseMock, instrumentsStorageMock, logosStorageMock, grpcClientMock);
    }

    void TearDown() override
    {
        delete thread;
        delete operationsDatabaseMock;
        delete instrumentsStorageMock;
        delete logosStorageMock;
        delete grpcClientMock;
    }

    OperationsThread*                   thread;
    StrictMock<OperationsDatabaseMock>* operationsDatabaseMock;
    StrictMock<InstrumentsStorageMock>* instrumentsStorageMock;
    StrictMock<LogosStorageMock>*       logosStorageMock;
    StrictMock<GrpcClientMock>*         grpcClientMock;
};



TEST_F(Test_OperationsThread, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsThread, Test_run)
{
    const InSequence seq;

    EXPECT_CALL(*operationsDatabaseMock, setAccount(QString("account-hash")));

    thread->setAccountId("account-hash", "account-id");

    QList<Operation> operations;

    const std::shared_ptr<tinkoff::PositionsResponse> positionsResponse(new tinkoff::PositionsResponse());

    tinkoff::MoneyValue* money1 = positionsResponse->add_money(); // positionsResponse will take ownership
    money1->set_currency("usd");
    money1->set_units(0);
    money1->set_nano(0);

    tinkoff::MoneyValue* money2 = positionsResponse->add_money(); // positionsResponse will take ownership
    money2->set_currency("rub");
    money2->set_units(0);
    money2->set_nano(0);

    std::shared_ptr<PositionsStream> positionsStream(new PositionsStream());

    const std::shared_ptr<tinkoff::PositionsStreamResponse> positionsStreamResponse(new tinkoff::PositionsStreamResponse());

    tinkoff::PositionData* positionData = new tinkoff::PositionData(); // positionsStreamResponse will take ownership

    tinkoff::PositionsMoney* positionsMoney1 = positionData->add_money(); // positionData will take ownership
    tinkoff::MoneyValue*     money3          = new tinkoff::MoneyValue(); // positionsMoney1 will take ownership
    money3->set_currency("usd");
    money3->set_units(0);
    money3->set_nano(0);
    positionsMoney1->set_allocated_available_value(money3);

    tinkoff::PositionsMoney* positionsMoney2 = positionData->add_money(); // positionData will take ownership
    tinkoff::MoneyValue*     money4          = new tinkoff::MoneyValue(); // positionsMoney2 will take ownership
    money4->set_currency("rub");
    money4->set_units(15);
    money4->set_nano(0);
    positionsMoney2->set_allocated_available_value(money4);

    positionsStreamResponse->set_allocated_position(positionData);

    EXPECT_CALL(*operationsDatabaseMock, readOperations()).WillOnce(Return(operations));
    EXPECT_CALL(*grpcClientMock, getPositions(QThread::currentThread(), QString("account-id")))
        .WillOnce(Return(positionsResponse));
    EXPECT_CALL(*grpcClientMock, createPositionsStream(QString("account-id"))).WillOnce(Return(positionsStream));
    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 0, Gt(1704056400000), QString(""))
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, readPositionsStream(positionsStream)).WillOnce(Return(positionsStreamResponse));
    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 0, Gt(1704056400000), QString(""))
    )
        .WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, readPositionsStream(positionsStream)).WillOnce(Return(nullptr));
    EXPECT_CALL(*grpcClientMock, finishPositionsStream(positionsStream));

    thread->run();
}

TEST_F(Test_OperationsThread, Test_terminateThread)
{
    const InSequence seq;

    EXPECT_CALL(*operationsDatabaseMock, setAccount(QString("account-hash")));

    thread->setAccountId("account-hash", "account-id");

    std::shared_ptr<PositionsStream> positionsStream(new PositionsStream());
    EXPECT_CALL(*grpcClientMock, createPositionsStream(QString("account-id"))).WillOnce(Return(positionsStream));

    thread->createPositionsStream();

    EXPECT_CALL(*grpcClientMock, cancelPositionsStream(positionsStream));

    thread->terminateThread();
}

TEST_F(Test_OperationsThread, Test_requestOperations)
{
    const InSequence seq;

    EXPECT_CALL(*operationsDatabaseMock, setAccount(QString("account-hash")));

    thread->setAccountId("account-hash", "account-id");

    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> getOperationsByCursorResponse1(
        new tinkoff::GetOperationsByCursorResponse()
    );
    const std::shared_ptr<tinkoff::GetOperationsByCursorResponse> getOperationsByCursorResponse2(
        new tinkoff::GetOperationsByCursorResponse()
    );

    getOperationsByCursorResponse1->set_has_next(true);
    getOperationsByCursorResponse1->set_next_cursor("next-cursor");
    tinkoff::OperationItem* operationItem1 =
        getOperationsByCursorResponse1->add_items(); // getOperationsByCursorResponse1 will take ownership

    google::protobuf::Timestamp* dateTimestamp1 = new google::protobuf::Timestamp(); // operationItem1 will take ownership

    dateTimestamp1->set_seconds(1704056460);
    dateTimestamp1->set_nanos(0);

    tinkoff::MoneyValue* price1 = new tinkoff::MoneyValue(); // operationItem1 will take ownership

    price1->set_units(253);
    price1->set_nano(300000000);

    tinkoff::MoneyValue* payment1 = new tinkoff::MoneyValue(); // operationItem1 will take ownership

    payment1->set_units(-2533);
    payment1->set_nano(0);

    tinkoff::MoneyValue* commission1 = new tinkoff::MoneyValue(); // operationItem1 will take ownership

    commission1->set_units(-1);
    commission1->set_nano(-266500000);

    operationItem1->set_allocated_date(dateTimestamp1);
    operationItem1->set_type(tinkoff::OPERATION_TYPE_BUY);
    operationItem1->set_instrument_uid("aaaaa");
    operationItem1->set_position_uid("position-id");
    operationItem1->set_description("Buy 10 ivashka durashka shares");
    operationItem1->set_allocated_price(price1);
    operationItem1->set_quantity_done(10);
    operationItem1->set_allocated_payment(payment1);
    operationItem1->set_allocated_commission(commission1);

    getOperationsByCursorResponse2->set_has_next(false);
    tinkoff::OperationItem* operationItem2 =
        getOperationsByCursorResponse2->add_items(); // getOperationsByCursorResponse2 will take ownership

    google::protobuf::Timestamp* dateTimestamp2 = new google::protobuf::Timestamp(); // operationItem2 will take ownership

    dateTimestamp2->set_seconds(1704056400);
    dateTimestamp2->set_nanos(0);

    tinkoff::MoneyValue* price2 = new tinkoff::MoneyValue(); // operationItem2 will take ownership

    price2->set_units(0);
    price2->set_nano(0);

    tinkoff::MoneyValue* payment2 = new tinkoff::MoneyValue(); // operationItem2 will take ownership

    payment2->set_units(200000);
    payment2->set_nano(0);

    tinkoff::MoneyValue* commission2 = new tinkoff::MoneyValue(); // operationItem2 will take ownership

    commission2->set_units(0);
    commission2->set_nano(0);

    operationItem2->set_allocated_date(dateTimestamp2);
    operationItem2->set_type(tinkoff::OPERATION_TYPE_INPUT);
    operationItem2->set_instrument_uid(RUBLE_UID);
    operationItem2->set_position_uid("");
    operationItem2->set_description("Give more money");
    operationItem2->set_allocated_price(price2);
    operationItem2->set_quantity_done(0);
    operationItem2->set_allocated_payment(payment2);
    operationItem2->set_allocated_commission(commission2);

    Instruments instruments;
    Logo        logo;

    QList<Operation> operations;

    Operation operation1;
    Operation operation2;

    operation1.timestamp                       = 1704056460000;
    operation1.instrumentId                    = "aaaaa";
    operation1.instrumentTicker                = "aaaaa";
    operation1.instrumentName                  = "?????";
    operation1.description                     = "Buy 10 ivashka durashka shares";
    operation1.price                           = 253.3f;
    operation1.avgPriceFifo                    = 253.3f;
    operation1.avgPriceWavg                    = 253.3f;
    operation1.quantity                        = 10;
    operation1.remainedQuantity                = 10;
    operation1.payment                         = -2533.0f;
    operation1.avgCostFifo                     = 2533.0f;
    operation1.costFifo.units                  = 2533;
    operation1.costFifo.nano                   = 0;
    operation1.costWavg.units                  = 2533;
    operation1.costWavg.nano                   = 0;
    operation1.commission                      = -1.2665f;
    operation1.yield                           = 0.0f;
    operation1.yieldWithCommission             = -1.2665f;
    operation1.yieldWithCommissionPercent      = -0.05f;
    operation1.inputMoney.units                = 200000;
    operation1.inputMoney.nano                 = 0;
    operation1.maxInputMoney.units             = 200000;
    operation1.maxInputMoney.nano              = 0;
    operation1.totalYieldWithCommission.units  = -1;
    operation1.totalYieldWithCommission.nano   = -266500000;
    operation1.totalYieldWithCommissionPercent = -0.0006332f;
    operation1.remainedMoney.units             = 197466;
    operation1.remainedMoney.nano              = -266500000;
    operation1.totalMoney.units                = 199999;
    operation1.totalMoney.nano                 = -266500000;
    operation1.pricePrecision                  = 2;
    operation1.paymentPrecision                = 2;
    operation1.commissionPrecision             = 4;

    OperationFifoItem item;

    item.quantity   = 10;
    item.cost.units = 253;
    item.cost.nano  = 300000000;

    operation1.fifoItems.append(item);

    operation2.timestamp                       = 1704056400000;
    operation2.instrumentId                    = RUBLE_UID;
    operation2.instrumentTicker                = RUBLE_UID;
    operation2.instrumentName                  = "?????";
    operation2.description                     = "Give more money";
    operation2.price                           = 0.0f;
    operation2.avgPriceFifo                    = 0.0f;
    operation2.avgPriceWavg                    = 0.0f;
    operation2.quantity                        = 0;
    operation2.remainedQuantity                = 0;
    operation2.payment                         = 200000.0f;
    operation2.avgCostFifo                     = 0.0f;
    operation2.costFifo.units                  = 0;
    operation2.costFifo.nano                   = 0;
    operation2.costWavg.units                  = 0;
    operation2.costWavg.nano                   = 0;
    operation2.commission                      = 0.0f;
    operation2.yield                           = 0.0f;
    operation2.yieldWithCommission             = 0.0f;
    operation2.yieldWithCommissionPercent      = 0.0f;
    operation2.inputMoney.units                = 200000;
    operation2.inputMoney.nano                 = 0;
    operation2.maxInputMoney.units             = 200000;
    operation2.maxInputMoney.nano              = 0;
    operation2.totalYieldWithCommission.units  = 0;
    operation2.totalYieldWithCommission.nano   = 0;
    operation2.totalYieldWithCommissionPercent = 0.0f;
    operation2.remainedMoney.units             = 200000;
    operation2.remainedMoney.nano              = 0;
    operation2.totalMoney.units                = 200000;
    operation2.totalMoney.nano                 = 0;
    operation2.pricePrecision                  = 2;
    operation2.paymentPrecision                = 2;
    operation2.commissionPrecision             = 2;

    operations.append(operation1);
    operations.append(operation2);

    EXPECT_CALL(
        *grpcClientMock, getOperations(QThread::currentThread(), QString("account-id"), 0, Gt(1704056400000), QString(""))
    )
        .WillOnce(Return(getOperationsByCursorResponse1));
    EXPECT_CALL(
        *grpcClientMock,
        getOperations(QThread::currentThread(), QString("account-id"), 0, Gt(1704056400000), QString("next-cursor"))
    )
        .WillOnce(Return(getOperationsByCursorResponse2));
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString(RUBLE_UID))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*instrumentsStorageMock, readLock());
    EXPECT_CALL(*instrumentsStorageMock, getInstruments()).WillOnce(ReturnRef(instruments));
    EXPECT_CALL(*instrumentsStorageMock, readUnlock());
    EXPECT_CALL(*logosStorageMock, readLock());
    EXPECT_CALL(*logosStorageMock, getLogo(QString("aaaaa"))).WillOnce(Return(&logo));
    EXPECT_CALL(*logosStorageMock, readUnlock());
    EXPECT_CALL(*grpcClientMock, getPortfolio(QThread::currentThread(), QString("account-id"))).WillOnce(Return(nullptr));
    EXPECT_CALL(*operationsDatabaseMock, writeOperations(operations));

    thread->requestOperations();
}
