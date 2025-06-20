#include "src/widgets/tablemodels/portfoliotreemodel/portfoliotreemodel.h"

#include <gtest/gtest.h>



class Test_PortfolioTreeModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        model = new PortfolioTreeModel();
    }

    void TearDown() override
    {
        delete model;
    }

    PortfolioTreeModel* model;
};



TEST_F(Test_PortfolioTreeModel, Test_constructor_and_destructor)
{
}
