#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <gtest/gtest.h>



class Test_StocksTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new StocksTableModel();
    }

    void TearDown() override
    {
        delete model;
    }

    StocksTableModel* model;
};



TEST_F(Test_StocksTableModel, Test_constructor_and_destructor)
{
}
