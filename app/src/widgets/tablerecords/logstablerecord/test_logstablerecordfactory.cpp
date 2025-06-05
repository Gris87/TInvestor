#include "src/widgets/tablerecords/logstablerecord/logstablerecordfactory.h"

#include <gtest/gtest.h>

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

    StrictMock<LogLevelTableItemWidgetFactoryMock> logLevelTableItemWidgetFactoryMock;

    // tableWidget will take ownership
    StrictMock<LogLevelTableItemWidgetMock>* logLevelTableItemWidgetMock = new StrictMock<LogLevelTableItemWidgetMock>(nullptr);

    QTableWidget tableWidget;

    tableWidget.setColumnCount(LOGS_COLUMN_COUNT);

    EXPECT_CALL(logLevelTableItemWidgetFactoryMock, newInstance(&tableWidget)).WillOnce(Return(logLevelTableItemWidgetMock));

    const ILogsTableRecord* record = factory.newInstance(&tableWidget, &logLevelTableItemWidgetFactoryMock, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
