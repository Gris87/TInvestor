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

        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        instrumentTableItemWidgetMock        = new StrictMock<InstrumentTableItemWidgetMock>(); // tableWidget will take ownership
        tableWidget                          = new QTableWidget();

        tableWidget->setColumnCount(OPERATIONS_COLUMN_COUNT);

        EXPECT_CALL(*instrumentTableItemWidgetFactoryMock, newInstance(userStorageMock, tableWidget))
            .WillOnce(Return(instrumentTableItemWidgetMock));

        record =
            new OperationsTableRecord(tableWidget, instrumentTableItemWidgetFactoryMock, userStorageMock, instrumentsStorageMock);
    }

    void TearDown() override
    {
        delete record;
        delete instrumentTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        // It will be deleted by tableWidget
        /*
        delete instrumentTableItemWidgetMock;
        */
        delete tableWidget;
    }

    OperationsTableRecord*                            record;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<InstrumentTableItemWidgetMock>*        instrumentTableItemWidgetMock;
    QTableWidget*                                     tableWidget;
};



TEST_F(Test_OperationsTableRecord, Test_constructor_and_destructor)
{
}
