#include "src/widgets/tablemodels/logstablemodel/logstablemodelfactory.h"

#include <gtest/gtest.h>



TEST(Test_LogsTableModelFactory, Test_constructor_and_destructor)
{
    const LogsTableModelFactory factory;
}

TEST(Test_LogsTableModelFactory, Test_newInstance)
{
    const LogsTableModelFactory factory;

    const ILogsTableModel* model = factory.newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
