#include "src/widgets/tablerecords/operationstablerecord/operationstablerecordfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/tableitems/stock/istocktableitemwidget_mock.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_OperationsTableRecordFactory, Test_constructor_and_destructor)
{
    const OperationsTableRecordFactory factory;
}

TEST(Test_OperationsTableRecordFactory, Test_newInstance)
{
    const InSequence seq;

    const OperationsTableRecordFactory factory;

    StrictMock<StockTableItemWidgetFactoryMock> stockTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                 userStorageMock;
    StrictMock<InstrumentsStorageMock>          instrumentsStorageMock;

    // tableWidget will take ownership
    StrictMock<StockTableItemWidgetMock>* stockTableItemWidgetMock = new StrictMock<StockTableItemWidgetMock>();

    QTableWidget tableWidget;

    tableWidget.setColumnCount(OPERATIONS_COLUMN_COUNT);

    EXPECT_CALL(stockTableItemWidgetFactoryMock, newInstance(&userStorageMock, &tableWidget))
        .WillOnce(Return(stockTableItemWidgetMock));

    const IOperationsTableRecord* record =
        factory.newInstance(&tableWidget, &stockTableItemWidgetFactoryMock, &userStorageMock, &instrumentsStorageMock, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
