#include "src/widgets/logstablewidget/logstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/storage/logos/ilogosstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



TEST(Test_LogsTableWidgetFactory, Test_constructor_and_destructor)
{
    const LogsTableWidgetFactory factory;
}

TEST(Test_LogsTableWidgetFactory, Test_newInstance)
{
    const InSequence seq;

    const LogsTableWidgetFactory factory;

    StrictMock<LogsTableModelFactoryMock> logsTableModelFactoryMock;
    StrictMock<LogosStorageMock>          logosStorageMock;
    StrictMock<FileDialogFactoryMock>     fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>        settingsEditorMock;

    StrictMock<LogsTableModelMock> logsTableModelMock;

    EXPECT_CALL(logsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(&logsTableModelMock));
    EXPECT_CALL(logsTableModelMock, rowCount(_)).WillRepeatedly(Return(0));
    EXPECT_CALL(logsTableModelMock, columnCount(_)).WillRepeatedly(Return(0));

    const ILogsTableWidget* widget =
        factory.newInstance(&logsTableModelFactoryMock, &logosStorageMock, &fileDialogFactoryMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
