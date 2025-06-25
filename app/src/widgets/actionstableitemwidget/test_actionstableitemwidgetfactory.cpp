#include "src/widgets/actionstableitemwidget/actionstableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory_mock.h"
#include "src/threads/orderbook/iorderbookthread_mock.h"
#include "src/utils/http/ihttpclient_mock.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_ActionsTableItemWidgetFactory, Test_constructor_and_destructor)
{
    const ActionsTableItemWidgetFactory factory;
}

TEST(Test_ActionsTableItemWidgetFactory, Test_newInstance)
{
    const ActionsTableItemWidgetFactory factory;

    StrictMock<OrderWavesDialogFactoryMock> orderWavesDialogFactoryMock;
    StrictMock<OrderWavesWidgetFactoryMock> orderWavesWidgetFactoryMock;
    StrictMock<OrderBookThreadMock>         orderBookThreadMock;
    StrictMock<HttpClientMock>              httpClientMock;
    Stock                                   stock;

    const IActionsTableItemWidget* widget = factory.newInstance(
        &orderWavesDialogFactoryMock, &orderWavesWidgetFactoryMock, &orderBookThreadMock, &httpClientMock, &stock, 2, nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
