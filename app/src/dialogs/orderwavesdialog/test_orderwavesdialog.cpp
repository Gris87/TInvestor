#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OrderWavesDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        orderWavesWidgetFactoryMock = new StrictMock<OrderWavesWidgetFactoryMock>();
        orderBookThreadMock         = new StrictMock<OrderBookThreadMock>();
        stock                       = new Stock();

        dialog = new OrderWavesDialog(orderWavesWidgetFactoryMock, orderBookThreadMock, stock);
    }

    void TearDown()
    {
        delete dialog;
        delete orderWavesWidgetFactoryMock;
        delete orderBookThreadMock;
        delete stock;
    }

    OrderWavesDialog*                        dialog;
    StrictMock<OrderWavesWidgetFactoryMock>* orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>*         orderBookThreadMock;
    Stock*                                   stock;
};



TEST_F(Test_OrderWavesDialog, Test_constructor_and_destructor)
{
}
