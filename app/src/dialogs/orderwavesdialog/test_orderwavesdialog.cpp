#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OrderWavesDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        orderBookThread = new StrictMock<OrderBookThreadMock>();
        stock           = new Stock();

        dialog = new OrderWavesDialog(orderBookThread, stock);
    }

    void TearDown()
    {
        delete dialog;
        delete orderBookThread;
        delete stock;
    }

    OrderWavesDialog*                dialog;
    StrictMock<OrderBookThreadMock>* orderBookThread;
    Stock*                           stock;
};



TEST_F(Test_OrderWavesDialog, Test_constructor_and_destructor)
{
}
