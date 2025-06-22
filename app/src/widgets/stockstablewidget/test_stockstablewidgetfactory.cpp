#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel_mock.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory_mock.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory_mock.h"

/*

using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_StocksTableWidgetFactory, Test_constructor_and_destructor)
{
    const StocksTableWidgetFactory factory;
}

TEST(Test_StocksTableWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    const StocksTableWidgetFactory factory;

    StrictMock<StocksTableModelFactoryMock>          stocksTableModelFactoryMock;
    StrictMock<StocksTableRecordFactoryMock>         stockTableRecordFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock> instrumentTableItemWidgetFactoryMock;
    StrictMock<ActionsTableItemWidgetFactoryMock>    actionsTableItemWidgetFactoryMock;
    StrictMock<OrderWavesDialogFactoryMock>          orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock>          orderWavesWidgetFactoryMock;
    StrictMock<LogosStorageMock>                     logosStorageMock;
    StrictMock<UserStorageMock>                      userStorageMock;
    StrictMock<OrderBookThreadMock>                  orderBookThreadMock;
    StrictMock<HttpClientMock>                       httpClientMock;
    StrictMock<FileDialogFactoryMock>                fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>                   settingsEditorMock;

    StrictMock<StocksTableModelMock> stocksTableModelMock;

    EXPECT_CALL(stocksTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(&stocksTableModelMock));
    EXPECT_CALL(stocksTableModelMock, rowCount(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(stocksTableModelMock, columnCount(_)).WillRepeatedly(Return(0));

    const IStocksTableWidget* widget = factory.newInstance(
        &stocksTableModelFactoryMock,
        &stockTableRecordFactoryMock,
        &instrumentTableItemWidgetFactoryMock,
        &actionsTableItemWidgetFactoryMock,
        &orderWavesDialogFactoryMock,
        &orderWavesWidgetFactoryMock,
        &logosStorageMock,
        &userStorageMock,
        &orderBookThreadMock,
        &httpClientMock,
        &fileDialogFactoryMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
*/
