#include "src/widgets/tablerecords/logstablerecord/logstablerecordfactory.h"

#include <gtest/gtest.h>



TEST(Test_LogsTableRecordFactory, Test_constructor_and_destructor)
{
    const LogsTableRecordFactory factory;
}

TEST(Test_LogsTableRecordFactory, Test_newInstance)
{
    const LogsTableRecordFactory factory;

    QTableWidget tableWidget;
    tableWidget.setColumnCount(LOGS_COLUMN_COUNT);

    const ILogsTableRecord* record = factory.newInstance(&tableWidget, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
