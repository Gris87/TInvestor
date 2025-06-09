#include "src/widgets/logstablewidget/logstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory_mock.h"



using ::testing::StrictMock;



TEST(Test_LogsTableWidgetFactory, Test_constructor_and_destructor)
{
    const LogsTableWidgetFactory factory;
}

TEST(Test_LogsTableWidgetFactory, Test_newInstance)
{
    const LogsTableWidgetFactory factory;

    StrictMock<LogsTableRecordFactoryMock>           logsTableRecordFactoryMock;
    StrictMock<LogLevelTableItemWidgetFactoryMock>   logLevelTableItemWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock> instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>                      userStorageMock;
    StrictMock<InstrumentsStorageMock>               instrumentsStorageMock;
    StrictMock<FileDialogFactoryMock>                fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>                   settingsEditorMock;

    const ILogsTableWidget* widget = factory.newInstance(
        &logsTableRecordFactoryMock,
        &logLevelTableItemWidgetFactoryMock,
        &instrumentTableItemWidgetFactoryMock,
        &userStorageMock,
        &instrumentsStorageMock,
        &fileDialogFactoryMock,
        &settingsEditorMock,
        nullptr
    );
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
