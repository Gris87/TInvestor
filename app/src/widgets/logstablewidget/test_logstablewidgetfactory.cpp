#include "src/widgets/logstablewidget/logstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_LogsTableWidgetFactory, Test_constructor_and_destructor)
{
    LogsTableWidgetFactory factory;
}

TEST(Test_LogsTableWidgetFactory, Test_newInstance)
{
    LogsTableWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    ILogsTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
