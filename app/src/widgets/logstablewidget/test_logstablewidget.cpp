#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



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
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        logsTableWidget = new LogsTableWidget(settingsEditorMock);
    }

    void TearDown() override
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

TEST_F(Test_LogsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"),    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Message"), _));
    // clang-format on

    logsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_LogsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),    QVariant(42))).WillOnce(Return(QVariant(42)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Message"), QVariant(72))).WillOnce(Return(QVariant(72)));
    // clang-format on

    logsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
