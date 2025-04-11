#include "src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/stocks/istocksstorage_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_StocksControlsWidgetFactory, Test_constructor_and_destructor)
{
    const StocksControlsWidgetFactory factory;
}

TEST(Test_StocksControlsWidgetFactory, Test_newInstance)
{
    const StocksControlsWidgetFactory factory;

    StrictMock<StocksStorageMock>  stocksStorageMock;
    StrictMock<SettingsEditorMock> settingsEditorMock;

    const IStocksControlsWidget* widget = factory.newInstance(&stocksStorageMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
