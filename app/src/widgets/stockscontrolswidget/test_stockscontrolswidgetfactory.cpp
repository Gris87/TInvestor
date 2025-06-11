#include "src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_StocksControlsWidgetFactory, Test_constructor_and_destructor)
{
    const StocksControlsWidgetFactory factory;
}

TEST(Test_StocksControlsWidgetFactory, Test_newInstance)
{
    const StocksControlsWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    const IStocksControlsWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
