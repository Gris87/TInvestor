#include "src/widgets/tablerecord/items/actions/actionstableitemwidget.h"
#include "ui_actionstableitemwidget.h"

#include <gtest/gtest.h>

#include "src/dialogs/marketwavesdialog/imarketwavesdialogfactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



class Test_ActionsTableItemWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        marketWavesDialogFactoryMock = new StrictMock<MarketWavesDialogFactoryMock>();
        httpClientMock               = new StrictMock<HttpClientMock>();
        stock                        = new Stock();

        stock->meta.uid                    = "aaa";
        stock->meta.ticker                 = "WAGA";
        stock->meta.name                   = "Wata Giga";
        stock->meta.minPriceIncrement.nano = 10000;

        widget = new ActionsTableItemWidget(marketWavesDialogFactoryMock, httpClientMock, stock);
    }

    void TearDown()
    {
        delete widget;
        delete marketWavesDialogFactoryMock;
        delete httpClientMock;
        delete stock;
    }

    ActionsTableItemWidget*                   widget;
    StrictMock<MarketWavesDialogFactoryMock>* marketWavesDialogFactoryMock;
    StrictMock<HttpClientMock>*               httpClientMock;
    Stock*                                    stock;
};



TEST_F(Test_ActionsTableItemWidget, Test_constructor_and_destructor)
{
}


TEST_F(Test_ActionsTableItemWidget, Test_on_linkButton_clicked)
{
    InSequence seq;

    EXPECT_CALL(*httpClientMock, openInBrowser(QUrl("https://www.tbank.ru/invest/stocks/WAGA/"))).WillOnce(Return(true));

    widget->ui->linkButton->click();
}
