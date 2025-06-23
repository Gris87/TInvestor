#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidget_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_OrderWavesDialogFactory, Test_constructor_and_destructor)
{
    const OrderWavesDialogFactory factory;
}

TEST(Test_OrderWavesDialogFactory, Test_newInstance)
{
    const InSequence seq;

    const OrderWavesDialogFactory factory;

    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    Stock                                   stock;

    StrictMock<OrderWavesWidgetMock>* orderWavesWidgetMock =
        new StrictMock<OrderWavesWidgetMock>(); // Will be deleted in OrderWavesDialog constructor

    stock.meta.instrumentName    = "Serezha stock";
    stock.meta.minPriceIncrement = 1.5f;
    stock.meta.pricePrecision    = 1;

    EXPECT_CALL(orderWavesWidgetFactoryMock, newInstance(2, FloatEq(1.5f), NotNull())).WillOnce(Return(orderWavesWidgetMock));
    EXPECT_CALL(orderBookThreadMock, setStock(&stock));
    EXPECT_CALL(orderBookThreadMock, run());
    EXPECT_CALL(orderBookThreadMock, terminateThread());

    const std::shared_ptr<IOrderWavesDialog> dialog =
        factory.newInstance(&orderWavesWidgetFactoryMock, &orderBookThreadMock, &stock, 2, nullptr);
    ASSERT_TRUE(dialog != nullptr);

    orderBookThreadMock.wait();
}
// NOLINTEND(readability-magic-numbers)
