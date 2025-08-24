#include "src/widgets/tablemodels/stockstablemodel/stockstablemodelfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



class Test_StocksTableModelFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new StocksTableModelFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    StocksTableModelFactory* factory;
};



TEST_F(Test_StocksTableModelFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksTableModelFactory, Test_newInstance)
{
    StrictMock<UserStorageMock> userStorageMock;

    const IStocksTableModel* model = factory->newInstance(&userStorageMock, nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
