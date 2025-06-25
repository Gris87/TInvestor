#include "src/widgets/tablemodels/stockstablemodel/stockstablemodelfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



TEST(Test_StocksTableModelFactory, Test_constructor_and_destructor)
{
    const StocksTableModelFactory factory;
}

TEST(Test_StocksTableModelFactory, Test_newInstance)
{
    const StocksTableModelFactory factory;

    StrictMock<UserStorageMock> userStorageMock;

    const IStocksTableModel* model = factory.newInstance(&userStorageMock, nullptr);
    ASSERT_TRUE(model != nullptr);

    delete model;
}
