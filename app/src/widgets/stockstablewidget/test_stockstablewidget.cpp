#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_StocksTableWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        stockTableRecordFactoryMock       = new StrictMock<StocksTableRecordFactoryMock>();
        stockTableItemWidgetFactoryMock   = new StrictMock<StockTableItemWidgetFactoryMock>();
        actionsTableItemWidgetFactoryMock = new StrictMock<ActionsTableItemWidgetFactoryMock>();
        orderWavesDialogFactoryMock       = new StrictMock<OrderWavesDialogFactoryMock>();
        orderWavesWidgetFactoryMock       = new StrictMock<OrderWavesWidgetFactoryMock>();
        userStorageMock                   = new StrictMock<UserStorageMock>();
        orderBookThreadMock               = new StrictMock<OrderBookThreadMock>();
        httpClientMock                    = new StrictMock<HttpClientMock>();
        settingsEditorMock                = new StrictMock<SettingsEditorMock>();

        stocksTableWidget = new StocksTableWidget(
            stockTableRecordFactoryMock,
            stockTableItemWidgetFactoryMock,
            actionsTableItemWidgetFactoryMock,
            orderWavesDialogFactoryMock,
            orderWavesWidgetFactoryMock,
            userStorageMock,
            orderBookThreadMock,
            httpClientMock,
            settingsEditorMock
        );
    }

    void TearDown()
    {
        delete stocksTableWidget;
        delete stockTableRecordFactoryMock;
        delete stockTableItemWidgetFactoryMock;
        delete actionsTableItemWidgetFactoryMock;
        delete orderWavesDialogFactoryMock;
        delete orderWavesWidgetFactoryMock;
        delete userStorageMock;
        delete orderBookThreadMock;
        delete httpClientMock;
        delete settingsEditorMock;
    }

    StocksTableWidget*                             stocksTableWidget;
    StrictMock<StocksTableRecordFactoryMock>*      stockTableRecordFactoryMock;
    StrictMock<StockTableItemWidgetFactoryMock>*   stockTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>* actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>*       orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>*       orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>*                   userStorageMock;
    StrictMock<OrderBookThreadMock>*               orderBookThreadMock;
    StrictMock<HttpClientMock>*                    httpClientMock;
    StrictMock<SettingsEditorMock>*                settingsEditorMock;
};



TEST_F(Test_StocksTableWidget, Test_constructor_and_destructor)
{
}
