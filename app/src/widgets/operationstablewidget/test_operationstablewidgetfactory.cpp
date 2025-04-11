#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_OperationsTableWidgetFactory, Test_constructor_and_destructor)
{
    const OperationsTableWidgetFactory factory;
}

TEST(Test_OperationsTableWidgetFactory, Test_newInstance)
{
    const OperationsTableWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    const IOperationsTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
