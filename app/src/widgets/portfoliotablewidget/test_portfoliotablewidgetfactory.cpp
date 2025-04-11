#include "src/widgets/portfoliotablewidget/portfoliotablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_PortfolioTableWidgetFactory, Test_constructor_and_destructor)
{
    const PortfolioTableWidgetFactory factory;
}

TEST(Test_PortfolioTableWidgetFactory, Test_newInstance)
{
    const PortfolioTableWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    const IPortfolioTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
