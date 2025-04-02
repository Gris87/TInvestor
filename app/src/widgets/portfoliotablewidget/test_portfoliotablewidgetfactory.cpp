#include "src/widgets/portfoliotablewidget/portfoliotablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_PortfolioTableWidgetFactory, Test_constructor_and_destructor)
{
    PortfolioTableWidgetFactory factory;
}

TEST(Test_PortfolioTableWidgetFactory, Test_newInstance)
{
    StrictMock<SettingsEditorMock> settingsEditorMock;

    PortfolioTableWidgetFactory factory;

    IPortfolioTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
