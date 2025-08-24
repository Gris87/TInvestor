#include "src/widgets/tablemodels/operationstablemodel/operationstablemodelfactory.h"

#include <gtest/gtest.h>



class Test_OperationsTableModelFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new OperationsTableModelFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    OperationsTableModelFactory* factory;
};



TEST_F(Test_OperationsTableModelFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableModelFactory, Test_newInstance)
{
    const IOperationsTableModel* model = factory->newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
