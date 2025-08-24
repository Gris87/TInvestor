#include "src/widgets/tablemodels/logstablemodel/logstablemodelfactory.h"

#include <gtest/gtest.h>



class Test_LogsTableModelFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new LogsTableModelFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    LogsTableModelFactory* factory;
};



TEST_F(Test_LogsTableModelFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsTableModelFactory, Test_newInstance)
{
    const ILogsTableModel* model = factory->newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
