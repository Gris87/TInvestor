#include "src/widgets/tablerecord/tablerecordfactory.h"

#include <gtest/gtest.h>



TEST(Test_TableRecordFactory, Test_constructor_and_destructor)
{
    TableRecordFactory factory;
}

TEST(Test_TableRecordFactory, Test_newInstance)
{
    TableRecordFactory factory;

    ITableRecord* record = factory.newInstance(nullptr, nullptr, nullptr);
    ASSERT_TRUE(record != nullptr);

    delete record;
}
