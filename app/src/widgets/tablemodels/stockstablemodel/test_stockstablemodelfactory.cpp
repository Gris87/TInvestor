#include "src/widgets/tablemodels/stockstablemodel/stockstablemodelfactory.h"

#include <gtest/gtest.h>



TEST(Test_StocksTableModelFactory, Test_constructor_and_destructor)
{
    const StocksTableModelFactory factory;
}

TEST(Test_StocksTableModelFactory, Test_newInstance)
{
    const StocksTableModelFactory factory;

    const IStocksTableModel* model = factory.newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
