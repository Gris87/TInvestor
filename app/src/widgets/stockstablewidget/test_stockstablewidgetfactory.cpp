#include "src/widgets/stockstablewidget/stockstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



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
    StrictMock<SettingsEditorMock> settingsEditorMock;

    StocksTableWidgetFactory factory;

    IStocksTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
