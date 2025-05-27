#include "src/widgets/portfoliotreewidget/portfoliotreewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_PortfolioTreeWidgetFactory, Test_constructor_and_destructor)
{
    const PortfolioTreeWidgetFactory factory;
}

TEST(Test_PortfolioTreeWidgetFactory, Test_newInstance)
{
    const PortfolioTreeWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    const IPortfolioTreeWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
