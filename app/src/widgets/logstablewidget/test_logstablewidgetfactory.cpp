#include "src/widgets/logstablewidget/logstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_LogsTableWidgetFactory, Test_constructor_and_destructor)
{
    const LogsTableWidgetFactory factory;
}

TEST(Test_LogsTableWidgetFactory, Test_newInstance)
{
    const LogsTableWidgetFactory factory;

    StrictMock<LogsTableRecordFactoryMock> logsTableRecordFactoryMock;
    StrictMock<FileDialogFactoryMock>      fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>         settingsEditorMock;

    const ILogsTableWidget* widget =
        factory.newInstance(&logsTableRecordFactoryMock, &fileDialogFactoryMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
