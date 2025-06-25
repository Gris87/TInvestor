#include "src/widgets/tablemodels/stockstablemodel/stockstablemodel.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::StrictMock;



class Test_StocksTableModel : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        model = new StocksTableModel(userStorageMock);
    }

    void TearDown() override
    {
        delete model;
        delete userStorageMock;
    }

    StocksTableModel* model;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_StocksTableModel, Test_constructor_and_destructor)
{
}
