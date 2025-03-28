#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_OrderWavesDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        InSequence seq;

        orderWavesWidgetFactoryMock = new StrictMock<OrderWavesWidgetFactoryMock>();
        orderBookThreadMock         = new StrictMock<OrderBookThreadMock>();
        stock                       = new Stock();

        stock->meta.minPriceIncrement.units = 1;
        stock->meta.minPriceIncrement.nano  = 500000000;

        EXPECT_CALL(*orderWavesWidgetFactoryMock, newInstance(2, FloatEq(1.5f), NotNull()));
        EXPECT_CALL(*orderBookThreadMock, setStock(stock));
        EXPECT_CALL(*orderBookThreadMock, run());

        dialog = new OrderWavesDialog(orderWavesWidgetFactoryMock, orderBookThreadMock, stock, 2);

        orderBookThreadMock->wait();
    }

    void TearDown()
    {
        InSequence seq;

        EXPECT_CALL(*orderBookThreadMock, terminateThread());

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
