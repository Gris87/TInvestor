#include "src/widgets/tablerecords/logstablerecord/logstablerecord.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidget_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



class Test_LogsTableRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        logLevelTableItemWidgetFactoryMock   = new StrictMock<LogLevelTableItemWidgetFactoryMock>();
        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        logLevelTableItemWidgetMock   = new StrictMock<LogLevelTableItemWidgetMock>(nullptr);   // tableWidget will take ownership
        instrumentTableItemWidgetMock = new StrictMock<InstrumentTableItemWidgetMock>(nullptr); // tableWidget will take ownership
        tableWidget                   = new QTableWidget();

        tableWidget->setColumnCount(LOGS_COLUMN_COUNT);

        EXPECT_CALL(*logLevelTableItemWidgetFactoryMock, newInstance(tableWidget)).WillOnce(Return(logLevelTableItemWidgetMock));
        EXPECT_CALL(*instrumentTableItemWidgetFactoryMock, newInstance(userStorageMock, tableWidget))
            .WillOnce(Return(instrumentTableItemWidgetMock));

        record = new LogsTableRecord(
            tableWidget,
            logLevelTableItemWidgetFactoryMock,
            instrumentTableItemWidgetFactoryMock,
            userStorageMock,
            instrumentsStorageMock
        );
    }

    void TearDown() override
    {
        delete record;
        delete logLevelTableItemWidgetFactoryMock;
        delete instrumentTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        // It will be deleted by tableWidget
        /*
        delete logLevelTableItemWidgetMock;
        delete instrumentTableItemWidgetMock;
        */
        delete tableWidget;
    }

    LogsTableRecord*                                  record;
    StrictMock<LogLevelTableItemWidgetFactoryMock>*   logLevelTableItemWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<LogLevelTableItemWidgetMock>*          logLevelTableItemWidgetMock;
    StrictMock<InstrumentTableItemWidgetMock>*        instrumentTableItemWidgetMock;
    QTableWidget*                                     tableWidget;
};



TEST_F(Test_LogsTableRecord, Test_constructor_and_destructor)
{
}
