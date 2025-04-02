#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_LogsTableWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        logsTableWidget = new LogsTableWidget(settingsEditorMock);
    }

    void TearDown()
    {
        delete logsTableWidget;
        delete settingsEditorMock;
    }

    LogsTableWidget*                logsTableWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_LogsTableWidget, Test_constructor_and_destructor)
{
}
