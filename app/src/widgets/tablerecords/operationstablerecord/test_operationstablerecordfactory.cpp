#include "src/widgets/tablerecords/operationstablerecord/operationstablerecordfactory.h"

#include <gtest/gtest.h>



TEST(Test_OperationsTableRecordFactory, Test_constructor_and_destructor)
{
    const OperationsTableRecordFactory factory;
}

TEST(Test_OperationsTableRecordFactory, Test_newInstance)
{
    const OperationsTableRecordFactory factory;

    QTableWidget tableWidget;

    tableWidget.setColumnCount(OPERATIONS_COLUMN_COUNT);

    const IOperationsTableRecord* record = factory.newInstance(&tableWidget, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
