#include "src/widgets/tablemodels/logstablemodel/logstablemodel.h"

#include <gtest/gtest.h>



class Test_LogsTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new LogsTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    LogsTableModel* model;
};



TEST_F(Test_LogsTableModel, Test_constructor_and_destructor)
{
}
