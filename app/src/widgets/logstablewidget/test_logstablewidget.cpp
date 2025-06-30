#include "src/widgets/logstablewidget/logstablewidget.h"
#include "ui_logstablewidget.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialog_mock.h"
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
        // const InSequence seq;

        logsTableModelFactoryMock = new StrictMock<LogsTableModelFactoryMock>();
        fileDialogFactoryMock     = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock        = new StrictMock<SettingsEditorMock>();

        logsTableModelMock = new StrictMock<LogsTableModelMock>();

        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_logs_table").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_logs_table");

        EXPECT_CALL(*logsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(logsTableModelMock));
        EXPECT_CALL(*logsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
        EXPECT_CALL(*logsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(LOGS_COLUMN_COUNT));

        logsTableWidget = new LogsTableWidget(logsTableModelFactoryMock, fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete logsTableWidget;
        delete logsTableModelFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete logsTableModelMock;

        QDir(appDir + "/test/dir_for_logs_table").removeRecursively();
    }

    LogsTableWidget*                       logsTableWidget;
    StrictMock<LogsTableModelFactoryMock>* logsTableModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*     fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*        settingsEditorMock;
    StrictMock<LogsTableModelMock>*        logsTableModelMock;
    QString                                appDir;
};



TEST_F(Test_LogsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_LogsTableWidget, Test_setFilter)
{
    const InSequence seq;

    const LogFilter filter;

    EXPECT_CALL(*logsTableModelMock, setFilter(filter));

    logsTableWidget->setFilter(filter);
}

TEST_F(Test_LogsTableWidget, Test_logsRead)
{
    const InSequence seq;

    const QList<LogEntry> entries;

    EXPECT_CALL(*logsTableModelMock, logsRead(entries));

    logsTableWidget->logsRead(entries);

    // clang-format off
    ASSERT_EQ(logsTableWidget->ui->tableView->horizontalHeader()->sortIndicatorSection(), LOGS_TIME_COLUMN);
    ASSERT_EQ(logsTableWidget->ui->tableView->horizontalHeader()->sortIndicatorOrder(),   Qt::DescendingOrder);
    // clang-format on
}

TEST_F(Test_LogsTableWidget, Test_logAdded)
{
    const InSequence seq;

    const LogEntry entry;

    EXPECT_CALL(*logsTableModelMock, logAdded(entry));

    logsTableWidget->logAdded(entry);
}

TEST_F(Test_LogsTableWidget, Test_on_tableView_customContextMenuRequested)
{
    const QPoint pos;

    logsTableWidget->on_tableView_customContextMenuRequested(pos);
}

TEST_F(Test_LogsTableWidget, Test_actionExportToExcelTriggered)
{
    const InSequence seq;

    StrictMock<FileDialogMock>* fileDialogMock =
        new StrictMock<FileDialogMock>(); // Will be deleted in actionExportToExcelTriggered

    QFile excelFile(appDir + "/test/dir_for_logs_table/excel.xlsx");

    ASSERT_EQ(excelFile.exists(), false);

    EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/LogsTableWidget/exportToExcelFile"), _))
        .WillOnce(Return(QVariant(appDir + "/test/dir_for_logs_table/excel.xlsx")));
    EXPECT_CALL(
        *fileDialogFactoryMock,
        newInstance(logsTableWidget, QString("Export"), appDir + "/test/dir_for_logs_table", QString("Excel file (*.xlsx)"))
    )
        .WillOnce(Return(std::shared_ptr<IFileDialog>(fileDialogMock)));
    EXPECT_CALL(*fileDialogMock, setAcceptMode(QFileDialog::AcceptSave));
    EXPECT_CALL(*fileDialogMock, setDefaultSuffix(QString("xlsx")));
    EXPECT_CALL(*fileDialogMock, selectFile(appDir + "/test/dir_for_logs_table/excel.xlsx"));
    EXPECT_CALL(*fileDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*fileDialogMock, selectedFiles())
        .WillOnce(Return(QStringList() << appDir + "/test/dir_for_logs_table/excel.xlsx"));
    EXPECT_CALL(
        *settingsEditorMock,
        setValue(
            QString("MainWindow/LogsTableWidget/exportToExcelFile"), QVariant(appDir + "/test/dir_for_logs_table/excel.xlsx")
        )
    );
    EXPECT_CALL(*logsTableModelMock, exportToExcel(_));

    logsTableWidget->actionExportToExcelTriggered();

    // clang-format off
    ASSERT_EQ(excelFile.exists(), true);
    ASSERT_GE(excelFile.size(),   6087);
    // clang-format on
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
