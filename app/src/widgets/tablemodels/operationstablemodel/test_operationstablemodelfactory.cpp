#include "src/widgets/tablemodels/operationstablemodel/operationstablemodelfactory.h"

#include <gtest/gtest.h>

#include "src/config/iconfig_mock.h"



using ::testing::Ge;
using ::testing::InSequence;
using ::testing::Ne;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



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
    StrictMock<ConfigMock> configMock;

    const IOperationsTableModel* model = factory->newInstance(&configMock, nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
