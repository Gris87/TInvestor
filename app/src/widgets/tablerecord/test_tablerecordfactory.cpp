#include "src/widgets/tablerecord/tablerecordfactory.h"

#include <gtest/gtest.h>



TEST(Test_TableRecordFactory, Test_constructor_and_destructor)
{
    TableRecordFactory factory;
}

TEST(Test_TableRecordFactory, Test_newInstance)
{
    TableRecordFactory factory;

    QTableWidget tableWidget;
    Stock        stock;

    ITableRecord* record = factory.newInstance(&tableWidget, &stock, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
