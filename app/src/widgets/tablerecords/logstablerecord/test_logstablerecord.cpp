#include "src/widgets/tablerecords/logstablerecord/logstablerecord.h"

#include <gtest/gtest.h>



class Test_LogsTableRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        tableWidget = new QTableWidget();
        tableWidget->setColumnCount(LOGS_COLUMN_COUNT);

        record = new LogsTableRecord(tableWidget);
    }

    void TearDown() override
    {
        delete record;
        delete tableWidget;
    }

    LogsTableRecord* record;
    QTableWidget*    tableWidget;
};



TEST_F(Test_LogsTableRecord, Test_constructor_and_destructor)
{
}
