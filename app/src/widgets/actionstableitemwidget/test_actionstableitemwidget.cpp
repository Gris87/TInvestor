#include "src/widgets/actionstableitemwidget/actionstableitemwidget.h"
#include "ui_actionstableitemwidget.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialog_mock.h"
#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tablemodels/modelroles.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel_mock.h"



using ::testing::_;
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
        stocksTableModelMock        = new StrictMock<StocksTableModelMock>();

        widget = new ActionsTableItemWidget(
            orderWavesDialogFactoryMock, orderWavesWidgetFactoryMock, orderBookThreadMock, httpClientMock, stocksTableModelMock, 0
        );
    }

    void TearDown() override
    {
        delete widget;
        delete orderWavesDialogFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        delete stocksTableModelMock;
    }

    ActionsTableItemWidget*                  widget;
    StrictMock<OrderWavesDialogFactoryMock>* orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>* orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>*         orderBookThreadMock;
    StrictMock<HttpClientMock>*              httpClientMock;
    StrictMock<StocksTableModelMock>*        stocksTableModelMock;
};



TEST_F(Test_ActionsTableItemWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_ActionsTableItemWidget, Test_on_orderWavesButton_clicked)
{
    const InSequence seq;

    StrictMock<OrderWavesDialogMock>* orderWavesDialogMock =
        new StrictMock<OrderWavesDialogMock>(); // Will be deleted in authFailed

    Stock stock;

    EXPECT_CALL(*stocksTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*stocksTableModelMock, columnCount(QModelIndex())).WillOnce(Return(STOCKS_COLUMN_COUNT));
    EXPECT_CALL(*stocksTableModelMock, data(_, ROLE_STOCK)).WillOnce(Return(QVariant(reinterpret_cast<qint64>(&stock))));
    EXPECT_CALL(*orderWavesDialogFactoryMock, newInstance(orderWavesWidgetFactoryMock, orderBookThreadMock, &stock, widget))
        .WillOnce(Return(std::shared_ptr<IOrderWavesDialog>(orderWavesDialogMock)));
    EXPECT_CALL(*orderWavesDialogMock, exec()).WillOnce(Return(QDialog::Accepted));

    widget->ui->orderWavesButton->click();
}

TEST_F(Test_ActionsTableItemWidget, Test_on_linkButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(*stocksTableModelMock, rowCount(QModelIndex())).WillOnce(Return(1));
    EXPECT_CALL(*stocksTableModelMock, columnCount(QModelIndex())).WillOnce(Return(STOCKS_COLUMN_COUNT));
    EXPECT_CALL(*stocksTableModelMock, data(_, Qt::DisplayRole)).WillOnce(Return(QVariant("WAGA")));
    EXPECT_CALL(*httpClientMock, openInBrowser(QUrl("https://www.tbank.ru/invest/stocks/WAGA/"))).WillOnce(Return(true));

    widget->ui->linkButton->click();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
