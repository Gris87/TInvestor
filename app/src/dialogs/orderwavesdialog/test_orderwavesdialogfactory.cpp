#include "src/dialogs/orderwavesdialog/orderwavesdialogfactory.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"



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

    StrictMock<OrderBookThreadMock> orderBookThread;
    Stock                             stock;

    std::shared_ptr<IOrderWavesDialog> dialog = factory.newInstance(&orderBookThread, &stock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
