#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodelfactory.h"

#include <gtest/gtest.h>



TEST(Test_PortfolioTreeModelFactory, Test_constructor_and_destructor)
{
    const PortfolioTreeModelFactory factory;
}

TEST(Test_PortfolioTreeModelFactory, Test_newInstance)
{
    const PortfolioTreeModelFactory factory;

    const IPortfolioTreeModel* model = factory.newInstance(nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
