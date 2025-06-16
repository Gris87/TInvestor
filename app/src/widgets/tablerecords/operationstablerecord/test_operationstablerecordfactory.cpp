#include "src/widgets/tablerecords/operationstablerecord/operationstablerecordfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"



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

    StrictMock<InstrumentTableItemWidgetFactoryMock> instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                      userStorageMock;
    StrictMock<InstrumentsStorageMock>               instrumentsStorageMock;

    // tableWidget will take ownership
    StrictMock<InstrumentTableItemWidgetMock>* instrumentTableItemWidgetMock =
        new StrictMock<InstrumentTableItemWidgetMock>(nullptr);

    QTableWidget tableWidget;

    tableWidget.setColumnCount(OPERATIONS_OLD_COLUMN_COUNT);

    EXPECT_CALL(instrumentTableItemWidgetFactoryMock, newInstance(&userStorageMock, &tableWidget))
        .WillOnce(Return(instrumentTableItemWidgetMock));

    const IOperationsTableRecord* record = factory.newInstance(
        &tableWidget, &instrumentTableItemWidgetFactoryMock, &userStorageMock, &instrumentsStorageMock, nullptr
    );
    ASSERT_TRUE(record != nullptr);

    delete record;
}
