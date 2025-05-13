#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <gtest/gtest.h>



class Test_OperationsTableRecord : public ::testing::Test
{
protected:
    void SetUp() override
    {
        tableWidget = new QTableWidget();

        tableWidget->setColumnCount(COLUMN_COUNT);

        record = new OperationsTableRecord(tableWidget);
    }

    void TearDown() override
    {
        delete record;
        delete tableWidget;
    }

    OperationsTableRecord* record;
    QTableWidget*          tableWidget;
};



TEST_F(Test_OperationsTableRecord, Test_constructor_and_destructor)
{
}
