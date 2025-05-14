#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OperationsTableRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        stockTableItemWidgetFactoryMock = new StrictMock<StockTableItemWidgetFactoryMock>();
        userStorageMock                 = new StrictMock<UserStorageMock>();
        instrumentsStorageMock          = new StrictMock<InstrumentsStorageMock>();
        stockTableItemWidgetMock        = new StrictMock<StockTableItemWidgetMock>(); // tableWidget will take ownership
        tableWidget                     = new QTableWidget();

        tableWidget->setColumnCount(OPERATIONS_COLUMN_COUNT);

        EXPECT_CALL(*stockTableItemWidgetFactoryMock, newInstance(userStorageMock, tableWidget))
            .WillOnce(Return(stockTableItemWidgetMock));

        record = new OperationsTableRecord(tableWidget, stockTableItemWidgetFactoryMock, userStorageMock, instrumentsStorageMock);
    }

    void TearDown() override
    {
        delete record;
        delete stockTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        // It will be deleted by tableWidget
        /*
        delete stockTableItemWidgetMock;
        */
        delete tableWidget;
    }

    OperationsTableRecord*                       record;
    StrictMock<StockTableItemWidgetFactoryMock>* stockTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                 userStorageMock;
    StrictMock<InstrumentsStorageMock>*          instrumentsStorageMock;
    StrictMock<StockTableItemWidgetMock>*        stockTableItemWidgetMock;
    QTableWidget*                                tableWidget;
};



TEST_F(Test_OperationsTableRecord, Test_constructor_and_destructor)
{
}
