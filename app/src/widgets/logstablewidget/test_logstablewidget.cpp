#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel_mock.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory_mock.h"



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
        const InSequence seq;

        logsTableModelFactoryMock = new StrictMock<LogsTableModelFactoryMock>();
        fileDialogFactoryMock     = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock        = new StrictMock<SettingsEditorMock>();

        logsTableModelMock = new StrictMock<LogsTableModelMock>();

        EXPECT_CALL(*logsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(logsTableModelMock));
        EXPECT_CALL(*logsTableModelMock, rowCount(_)).WillRepeatedly(Return(0));
        EXPECT_CALL(*logsTableModelMock, columnCount(_)).WillRepeatedly(Return(0));

        logsTableWidget = new LogsTableWidget(logsTableModelFactoryMock, fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete logsTableWidget;
        delete logsTableModelFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete logsTableModelMock;
    }

    LogsTableWidget*                       logsTableWidget;
    StrictMock<LogsTableModelFactoryMock>* logsTableModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*     fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*        settingsEditorMock;
    StrictMock<LogsTableModelMock>*        logsTableModelMock;
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
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),    _)).WillOnce(Return(QVariant(110)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Level"),   _)).WillOnce(Return(QVariant(32)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),    _)).WillOnce(Return(QVariant(103)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Message"), _)).WillOnce(Return(QVariant(900)));
    // clang-format on

    logsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
