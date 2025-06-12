#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <gtest/gtest.h>

#include "src/storage/instruments/iinstrumentsstorage_mock.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory_mock.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_LogsTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsTableModelFactoryMock            = new StrictMock<LogsTableModelFactoryMock>();
        logsTableRecordFactoryMock           = new StrictMock<LogsTableRecordFactoryMock>();
        logLevelTableItemWidgetFactoryMock   = new StrictMock<LogLevelTableItemWidgetFactoryMock>();
        instrumentTableItemWidgetFactoryMock = new StrictMock<InstrumentTableItemWidgetFactoryMock>();
        userStorageMock                      = new StrictMock<UserStorageMock>();
        instrumentsStorageMock               = new StrictMock<InstrumentsStorageMock>();
        fileDialogFactoryMock                = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock                   = new StrictMock<SettingsEditorMock>();

        logsTableModelMock = new StrictMock<LogsTableModelMock>();

        EXPECT_CALL(*logsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(logsTableModelMock));
        EXPECT_CALL(*logsTableModelMock, rowCount(_)).WillRepeatedly(Return(0));
        EXPECT_CALL(*logsTableModelMock, columnCount(_)).WillRepeatedly(Return(0));

        logsTableWidget = new LogsTableWidget(
            logsTableModelFactoryMock,
            logsTableRecordFactoryMock,
            logLevelTableItemWidgetFactoryMock,
            instrumentTableItemWidgetFactoryMock,
            userStorageMock,
            instrumentsStorageMock,
            fileDialogFactoryMock,
            settingsEditorMock
        );
    }

    void TearDown() override
    {
        delete logsTableWidget;
        delete logsTableModelFactoryMock;
        delete logsTableRecordFactoryMock;
        delete logLevelTableItemWidgetFactoryMock;
        delete instrumentTableItemWidgetFactoryMock;
        delete userStorageMock;
        delete instrumentsStorageMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete logsTableModelMock;
    }

    LogsTableWidget*                                  logsTableWidget;
    StrictMock<LogsTableModelFactoryMock>*            logsTableModelFactoryMock;
    StrictMock<LogsTableRecordFactoryMock>*           logsTableRecordFactoryMock;
    StrictMock<LogLevelTableItemWidgetFactoryMock>*   logLevelTableItemWidgetFactoryMock;
    StrictMock<InstrumentTableItemWidgetFactoryMock>* instrumentTableItemWidgetFactoryMock;
    StrictMock<UserStorageMock>*                      userStorageMock;
    StrictMock<InstrumentsStorageMock>*               instrumentsStorageMock;
    StrictMock<FileDialogFactoryMock>*                fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*                   settingsEditorMock;
    StrictMock<LogsTableModelMock>*                   logsTableModelMock;
};



TEST_F(Test_LogsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Level"),   _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Message"), _));
    // clang-format on

    logsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_LogsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),    _)).WillOnce(Return(QVariant(64)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Level"),   _)).WillOnce(Return(QVariant(94)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),    _)).WillOnce(Return(QVariant(94)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Message"), _)).WillOnce(Return(QVariant(94)));
    // clang-format on

    logsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
