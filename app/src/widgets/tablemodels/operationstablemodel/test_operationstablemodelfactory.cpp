#include "src/widgets/tablemodels/operationstablemodel/operationstablemodelfactory.h"

#include <gtest/gtest.h>



TEST(Test_OperationsTableModelFactory, Test_constructor_and_destructor)
{
    const OperationsTableModelFactory factory;
}

TEST(Test_OperationsTableModelFactory, Test_newInstance)
{
    const OperationsTableModelFactory factory;

    const IOperationsTableModel* model = factory.newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
