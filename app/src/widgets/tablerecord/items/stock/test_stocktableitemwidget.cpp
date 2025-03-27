#include "src/widgets/tablerecord/items/stock/stocktableitemwidget.h"
#include "ui_stocktableitemwidget.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_StockTableItemWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        widget = new StockTableItemWidget(userStorageMock);
    }

    void TearDown()
    {
        delete widget;
        delete userStorageMock;
    }

    StockTableItemWidget*        widget;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_StockTableItemWidget, Test_constructor_and_destructor)
{
}
