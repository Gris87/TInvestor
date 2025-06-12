#include "src/widgets/tablerecords/logstablerecord/logstablerecordfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidget_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidget_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_LogsTableRecordFactory, Test_constructor_and_destructor)
{
    const LogsTableRecordFactory factory;
}

TEST(Test_LogsTableRecordFactory, Test_newInstance)
{
    const InSequence seq;

    const LogsTableRecordFactory factory;

    StrictMock<LogLevelTableItemWidgetFactoryMock>   logLevelTableItemWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock> instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                      userStorageMock;
    StrictMock<InstrumentsStorageMock>               instrumentsStorageMock;

    // tableWidget will take ownership
    StrictMock<LogLevelTableItemWidgetMock>*   logLevelTableItemWidgetMock = new StrictMock<LogLevelTableItemWidgetMock>(nullptr);
    StrictMock<InstrumentTableItemWidgetMock>* instrumentTableItemWidgetMock =
        new StrictMock<InstrumentTableItemWidgetMock>(nullptr);

    QTableWidget tableWidget;

    tableWidget.setColumnCount(LOGS_OLD_COLUMN_COUNT);

    EXPECT_CALL(logLevelTableItemWidgetFactoryMock, newInstance(&tableWidget)).WillOnce(Return(logLevelTableItemWidgetMock));
    EXPECT_CALL(instrumentTableItemWidgetFactoryMock, newInstance(&userStorageMock, &tableWidget))
        .WillOnce(Return(instrumentTableItemWidgetMock));

    const ILogsTableRecord* record = factory.newInstance(
        &tableWidget,
        &logLevelTableItemWidgetFactoryMock,
        &instrumentTableItemWidgetFactoryMock,
        &userStorageMock,
        &instrumentsStorageMock,
        nullptr
    );
    ASSERT_TRUE(record != nullptr);

    delete record;
}
