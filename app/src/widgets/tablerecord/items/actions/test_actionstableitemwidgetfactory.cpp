#include "src/widgets/tablerecord/items/actions/actionstableitemwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/http/ihttpclient_mock.h"



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

    StrictMock<HttpClientMock> httpClientMock;
    Stock                      stock;

    IActionsTableItemWidget* widget = factory.newInstance(&httpClientMock, &stock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
