#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_LogsTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        logsTableRecordFactoryMock = new StrictMock<LogsTableRecordFactoryMock>();
        fileDialogFactoryMock      = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock         = new StrictMock<SettingsEditorMock>();

        logsTableWidget = new LogsTableWidget(logsTableRecordFactoryMock, fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete logsTableWidget;
        delete logsTableRecordFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
    }

    LogsTableWidget*                        logsTableWidget;
    StrictMock<LogsTableRecordFactoryMock>* logsTableRecordFactoryMock;
    StrictMock<FileDialogFactoryMock>*      fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*         settingsEditorMock;
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
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Message"), _)).WillOnce(Return(QVariant(94)));
    // clang-format on

    logsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
