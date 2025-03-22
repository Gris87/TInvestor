#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



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
    OrderWavesDialogFactory factory;

    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    Stock                                   stock;

    std::shared_ptr<IOrderWavesDialog> dialog =
        factory.newInstance(&orderWavesWidgetFactoryMock, &orderBookThreadMock, &stock, 2, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
