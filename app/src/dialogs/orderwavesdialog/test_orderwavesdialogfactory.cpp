#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_OrderWavesDialogFactory, Test_constructor_and_destructor)
{
    OrderWavesDialogFactory factory;
}

TEST(Test_OrderWavesDialogFactory, Test_newInstance)
{
    const InSequence seq;

    OrderWavesDialogFactory factory;

    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    Stock                                   stock;

    stock.meta.minPriceIncrement.units = 1;
    stock.meta.minPriceIncrement.nano  = 500000000;

    EXPECT_CALL(orderWavesWidgetFactoryMock, newInstance(2, FloatEq(1.5f), NotNull()));
    EXPECT_CALL(orderBookThreadMock, setStock(&stock));
    EXPECT_CALL(orderBookThreadMock, run());
    EXPECT_CALL(orderBookThreadMock, terminateThread());

    std::shared_ptr<IOrderWavesDialog> dialog =
        factory.newInstance(&orderWavesWidgetFactoryMock, &orderBookThreadMock, &stock, 2, nullptr);
    ASSERT_TRUE(dialog != nullptr);

    orderBookThreadMock.wait();
}
