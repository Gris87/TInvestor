#include "src/widgets/tableitems/actions/actionstableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_ActionsTableItemWidgetFactory, Test_constructor_and_destructor)
{
    ActionsTableItemWidgetFactory factory;
}

TEST(Test_ActionsTableItemWidgetFactory, Test_newInstance)
{
    ActionsTableItemWidgetFactory factory;

    StrictMock<OrderWavesDialogFactoryMock> orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    StrictMock<HttpClientMock>              httpClientMock;
    Stock                                   stock;

    IActionsTableItemWidget* widget = factory.newInstance(
        &orderWavesDialogFactoryMock, &orderWavesWidgetFactoryMock, &orderBookThreadMock, &httpClientMock, &stock, 2, nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
