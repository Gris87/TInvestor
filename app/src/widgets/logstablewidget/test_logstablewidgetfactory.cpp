#include "src/widgets/logstablewidget/logstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_LogsTableWidgetFactory, Test_constructor_and_destructor)
{
    const LogsTableWidgetFactory factory;
}

TEST(Test_LogsTableWidgetFactory, Test_newInstance)
{
    const LogsTableWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    const ILogsTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
