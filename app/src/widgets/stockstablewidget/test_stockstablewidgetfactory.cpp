#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tablerecord/itablerecordfactory_mock.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecord/items/stock/istocktableitemwidgetfactory_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_StocksTableWidgetFactory, Test_constructor_and_destructor)
{
    StocksTableWidgetFactory factory;
}

TEST(Test_StocksTableWidgetFactory, Test_newInstance)
{
    StrictMock<TableRecordFactoryMock>            tableRecordFactoryMock;
    StrictMock<StockTableItemWidgetFactoryMock>   stockTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock> actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>       orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>       orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>                   userStorageMock;
    StrictMock<OrderBookThreadMock>               orderBookThreadMock;
    StrictMock<HttpClientMock>                    httpClientMock;
    StrictMock<SettingsEditorMock>                settingsEditorMock;

    StocksTableWidgetFactory factory;

    IStocksTableWidget* widget = factory.newInstance(
        &tableRecordFactoryMock,
        &stockTableItemWidgetFactoryMock,
        &actionsTableItemWidgetFactoryMock,
        &orderWavesDialogFactoryMock,
        &orderWavesWidgetFactoryMock,
        &userStorageMock,
        &orderBookThreadMock,
        &httpClientMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
