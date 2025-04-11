#include "src/widgets/tableitems/actions/actionstableitemwidget.h"
#include "ui_actionstableitemwidget.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialog_mock.h"
#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_ActionsTableItemWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        orderWavesDialogFactoryMock = new StrictMock<OrderWavesDialogFactoryMock>();
        orderWavesWidgetFactoryMock = new StrictMock<OrderWavesWidgetFactoryMock>();
        orderBookThreadMock         = new StrictMock<OrderBookThreadMock>();
        httpClientMock              = new StrictMock<HttpClientMock>();
        stock                       = new Stock();

        stock->meta.uid                    = "aaa";
        stock->meta.ticker                 = "WAGA";
        stock->meta.name                   = "Wata Giga";
        stock->meta.minPriceIncrement.nano = 10000;

        widget = new ActionsTableItemWidget(
            orderWavesDialogFactoryMock, orderWavesWidgetFactoryMock, orderBookThreadMock, httpClientMock, stock, 2
        );
    }

    void TearDown() override
    {
        delete widget;
        delete orderWavesDialogFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        delete stock;
    }

    ActionsTableItemWidget*                  widget;
    StrictMock<OrderWavesDialogFactoryMock>* orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>* orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>*         orderBookThreadMock;
    StrictMock<HttpClientMock>*              httpClientMock;
    Stock*                                   stock;
};



TEST_F(Test_ActionsTableItemWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_ActionsTableItemWidget, Test_on_orderWavesButton_clicked)
{
    const InSequence seq;

    StrictMock<OrderWavesDialogMock>* orderWavesDialogMock =
        new StrictMock<OrderWavesDialogMock>(); // Will be deleted in authFailed

    EXPECT_CALL(*orderWavesDialogFactoryMock, newInstance(orderWavesWidgetFactoryMock, orderBookThreadMock, stock, 2, widget))
        .WillOnce(Return(std::shared_ptr<IOrderWavesDialog>(orderWavesDialogMock)));
    EXPECT_CALL(*orderWavesDialogMock, exec()).WillOnce(Return(QDialog::Accepted));

    widget->ui->orderWavesButton->click();
}

TEST_F(Test_ActionsTableItemWidget, Test_on_linkButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*httpClientMock, openInBrowser(QUrl("https://www.tbank.ru/invest/stocks/WAGA/"))).WillOnce(Return(true));

    widget->ui->linkButton->click();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
