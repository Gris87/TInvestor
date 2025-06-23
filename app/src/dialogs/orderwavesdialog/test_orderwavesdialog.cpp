#include "src/dialogs/orderwavesdialog/orderwavesdialog.h"
#include "ui_orderwavesdialog.h"

#include <gtest/gtest.h>

#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidget_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::FloatEq;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Ref;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_OrderWavesDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        orderWavesWidgetFactoryMock = new StrictMock<OrderWavesWidgetFactoryMock>();
        orderWavesWidgetMock        = new StrictMock<OrderWavesWidgetMock>();
        orderBookThreadMock         = new StrictMock<OrderBookThreadMock>();
        stock                       = new Stock();

        stock->meta.instrumentName    = "Serezha stock";
        stock->meta.minPriceIncrement = 1.5f;

        EXPECT_CALL(*orderWavesWidgetFactoryMock, newInstance(2, FloatEq(1.5f), NotNull()))
            .WillOnce(Return(orderWavesWidgetMock));
        EXPECT_CALL(*orderBookThreadMock, setStock(stock));
        EXPECT_CALL(*orderBookThreadMock, run());

        dialog = new OrderWavesDialog(orderWavesWidgetFactoryMock, orderBookThreadMock, stock, 2);

        orderBookThreadMock->wait();
    }

    void TearDown() override
    {
        const InSequence seq;

        EXPECT_CALL(*orderBookThreadMock, terminateThread());

        delete dialog;
        delete orderWavesWidgetFactoryMock;
        // It will be deleted by `delete ui;`
        /*
        delete orderWavesWidgetMock;
        */
        delete orderBookThreadMock;
        delete stock;
    }

    OrderWavesDialog*                        dialog;
    StrictMock<OrderWavesWidgetFactoryMock>* orderWavesWidgetFactoryMock;
    StrictMock<OrderWavesWidgetMock>*        orderWavesWidgetMock;
    StrictMock<OrderBookThreadMock>*         orderBookThreadMock;
    Stock*                                   stock;
};



TEST_F(Test_OrderWavesDialog, Test_constructor_and_destructor)
{
    ASSERT_EQ(dialog->ui->nameLabel->text(), "Serezha stock");
}

TEST_F(Test_OrderWavesDialog, Test_orderBookChanged)
{
    const InSequence seq;

    OrderBook orderBook;

    orderBook.timestamp = 1704056400000;
    orderBook.price     = 1500.75f;

    EXPECT_CALL(*orderWavesWidgetMock, orderBookChanged(Ref(orderBook)));

    dialog->orderBookChanged(orderBook);

    // clang-format off
    ASSERT_EQ(dialog->ui->timeLabel->text(),  "2024-01-01 00:00:00");
    ASSERT_EQ(dialog->ui->priceLabel->text(), QString("1500.75 \u20BD / Spread: -"));
    // clang-format on

    OrderBookData bid;
    OrderBookData ask;

    bid.price    = 1500.5f;
    bid.quantity = 1000;
    ask.price    = 1500.9f;
    ask.quantity = 100;

    orderBook.bids.append(bid);
    orderBook.asks.append(ask);

    EXPECT_CALL(*orderWavesWidgetMock, orderBookChanged(Ref(orderBook)));

    dialog->orderBookChanged(orderBook);

    // clang-format off
    ASSERT_EQ(dialog->ui->timeLabel->text(),  "2024-01-01 00:00:00");
    ASSERT_EQ(dialog->ui->priceLabel->text(), QString("1500.75 \u20BD / Spread: 0.40 \u20BD (0.027%)"));
    // clang-format on
}

TEST_F(Test_OrderWavesDialog, Test_on_resetButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*orderWavesWidgetMock, reset());

    dialog->ui->resetButton->click();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
