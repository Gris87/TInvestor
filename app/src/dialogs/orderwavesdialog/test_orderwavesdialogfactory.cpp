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
class Test_OrderWavesDialogFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new OrderWavesDialogFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    OrderWavesDialogFactory* factory;
};



TEST_F(Test_OrderWavesDialogFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_OrderWavesDialogFactory, Test_newInstance)
{
    const InSequence seq;

    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    Stock                                   stock;

    StrictMock<OrderWavesWidgetMock>* orderWavesWidgetMock =
        new StrictMock<OrderWavesWidgetMock>(); // Will be deleted in OrderWavesDialog constructor

    stock.meta.instrumentTicker            = "SERG";
    stock.meta.instrumentName              = "Serezha stock";
    stock.meta.forQualInvestorFlag         = false;
    stock.meta.lot                         = 1;
    stock.meta.pricePrecision              = 2;
    stock.meta.minPriceIncrement           = 1.5f;
    stock.meta.lastTradeTime               = QTime(18, 39);
    stock.meta.turnover                    = 9310000;
    stock.meta.dividends.createTimestamp   = 1735678800000;
    stock.meta.dividends.paymentTimestamp  = 1738357200000;
    stock.meta.dividends.yield             = 10.0f;
    stock.meta.shorts.enabled              = true;
    stock.meta.shorts.lastEnabledTimestamp = 1735678800000;

    EXPECT_CALL(orderWavesWidgetFactoryMock, newInstance(2, FloatEq(1.5f), NotNull())).WillOnce(Return(orderWavesWidgetMock));
    EXPECT_CALL(orderBookThreadMock, setStock(&stock));
    EXPECT_CALL(orderBookThreadMock, run());
    EXPECT_CALL(orderBookThreadMock, terminateThread());

    const std::shared_ptr<IOrderWavesDialog> dialog =
        factory->newInstance(&orderWavesWidgetFactoryMock, &orderBookThreadMock, &stock, nullptr);
    ASSERT_TRUE(dialog != nullptr);

    orderBookThreadMock.wait();
}
// NOLINTEND(readability-magic-numbers)
