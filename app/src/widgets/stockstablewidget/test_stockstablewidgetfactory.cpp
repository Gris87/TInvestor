#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_StocksTableWidgetFactory, Test_constructor_and_destructor)
{
    const StocksTableWidgetFactory factory;
}

TEST(Test_StocksTableWidgetFactory, Test_newInstance)
{
    const StocksTableWidgetFactory factory;

    StrictMock<StocksTableRecordFactoryMock>      stockTableRecordFactoryMock;
    StrictMock<StockTableItemWidgetFactoryMock>   stockTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock> actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>       orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>       orderWavesWidgetFactoryMock;
    StrictMock<UserStorageMock>                   userStorageMock;
    StrictMock<OrderBookThreadMock>               orderBookThreadMock;
    StrictMock<HttpClientMock>                    httpClientMock;
    StrictMock<SettingsEditorMock>                settingsEditorMock;

    const IStocksTableWidget* widget = factory.newInstance(
        &stockTableRecordFactoryMock,
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
