#include "src/widgets/tablemodels/operationstablemodel/operationstablemodel.h"

#include <gtest/gtest.h>



class Test_OperationsTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new OperationsTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    OperationsTableModel* model;
};



TEST_F(Test_OperationsTableModel, Test_constructor_and_destructor)
{
}
