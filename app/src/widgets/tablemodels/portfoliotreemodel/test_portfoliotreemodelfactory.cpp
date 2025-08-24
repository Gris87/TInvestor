#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodelfactory.h"

#include <gtest/gtest.h>



class Test_PortfolioTreeModelFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new PortfolioTreeModelFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    PortfolioTreeModelFactory* factory;
};



TEST_F(Test_PortfolioTreeModelFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_PortfolioTreeModelFactory, Test_newInstance)
{
    const IPortfolioTreeModel* model = factory->newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
