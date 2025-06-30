#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <QCoreApplication>
#include <QDir>
#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialog_mock.h"
#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodel_mock.h"
#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodelfactory_mock.h"



using ::testing::_;
using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_OperationsTableWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // const InSequence seq;

        operationsTableModelFactoryMock = new StrictMock<OperationsTableModelFactoryMock>();
        fileDialogFactoryMock           = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock              = new StrictMock<SettingsEditorMock>();

        operationsTableModelMock = new StrictMock<OperationsTableModelMock>();

        appDir = qApp->applicationDirPath();
        QDir(appDir + "/test/dir_for_operations_table").removeRecursively();
        QDir().mkpath(appDir + "/test/dir_for_operations_table");

        EXPECT_CALL(*operationsTableModelFactoryMock, newInstance(NotNull())).WillOnce(Return(operationsTableModelMock));
        EXPECT_CALL(*operationsTableModelMock, rowCount(QModelIndex())).WillRepeatedly(Return(0));
        EXPECT_CALL(*operationsTableModelMock, columnCount(QModelIndex())).WillRepeatedly(Return(OPERATIONS_COLUMN_COUNT));

        operationsTableWidget =
            new OperationsTableWidget(operationsTableModelFactoryMock, fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete operationsTableWidget;
        delete operationsTableModelFactoryMock;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
        delete operationsTableModelMock;

        QDir(appDir + "/test/dir_for_operations_table").removeRecursively();
    }

    OperationsTableWidget*                       operationsTableWidget;
    StrictMock<OperationsTableModelFactoryMock>* operationsTableModelFactoryMock;
    StrictMock<FileDialogFactoryMock>*           fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*              settingsEditorMock;
    StrictMock<OperationsTableModelMock>*        operationsTableModelMock;
    QString                                      appDir;
};



TEST_F(Test_OperationsTableWidget, Test_constructor_and_destructor)
{
}

TEST_F(Test_OperationsTableWidget, Test_operationsRead)
{
    const InSequence seq;

    const QList<Operation> operations;

    EXPECT_CALL(*operationsTableModelMock, operationsRead(operations));

    operationsTableWidget->operationsRead(operations);

    // clang-format off
    ASSERT_EQ(operationsTableWidget->ui->tableView->horizontalHeader()->sortIndicatorSection(), OPERATIONS_TIME_COLUMN);
    ASSERT_EQ(operationsTableWidget->ui->tableView->horizontalHeader()->sortIndicatorOrder(),   Qt::DescendingOrder);
    // clang-format on
}

TEST_F(Test_OperationsTableWidget, Test_operationsAdded)
{
    const InSequence seq;

    const QList<Operation> operations;

    EXPECT_CALL(*operationsTableModelMock, operationsAdded(operations));

    operationsTableWidget->operationsAdded(operations);
}

TEST_F(Test_OperationsTableWidget, Test_on_tableView_customContextMenuRequested)
{
    const QPoint pos;

    operationsTableWidget->on_tableView_customContextMenuRequested(pos);
}

TEST_F(Test_OperationsTableWidget, Test_actionExportToExcelTriggered)
{
    const InSequence seq;

    StrictMock<FileDialogMock>* fileDialogMock =
        new StrictMock<FileDialogMock>(); // Will be deleted in actionExportToExcelTriggered

    QFile excelFile(appDir + "/test/dir_for_operations_table/excel.xlsx");

    ASSERT_EQ(excelFile.exists(), false);

    EXPECT_CALL(*settingsEditorMock, value(QString("MainWindow/OperationsTableWidget/exportToExcelFile"), _))
        .WillOnce(Return(QVariant(appDir + "/test/dir_for_operations_table/excel.xlsx")));
    EXPECT_CALL(
        *fileDialogFactoryMock,
        newInstance(
            operationsTableWidget, QString("Export"), appDir + "/test/dir_for_operations_table", QString("Excel file (*.xlsx)")
        )
    )
        .WillOnce(Return(std::shared_ptr<IFileDialog>(fileDialogMock)));
    EXPECT_CALL(*fileDialogMock, setAcceptMode(QFileDialog::AcceptSave));
    EXPECT_CALL(*fileDialogMock, setDefaultSuffix(QString("xlsx")));
    EXPECT_CALL(*fileDialogMock, selectFile(appDir + "/test/dir_for_operations_table/excel.xlsx"));
    EXPECT_CALL(*fileDialogMock, exec()).WillOnce(Return(QDialog::Accepted));
    EXPECT_CALL(*fileDialogMock, selectedFiles())
        .WillOnce(Return(QStringList() << appDir + "/test/dir_for_operations_table/excel.xlsx"));
    EXPECT_CALL(
        *settingsEditorMock,
        setValue(
            QString("MainWindow/OperationsTableWidget/exportToExcelFile"),
            QVariant(appDir + "/test/dir_for_operations_table/excel.xlsx")
        )
    );
    EXPECT_CALL(*operationsTableModelMock, exportToExcel(_));

    operationsTableWidget->actionExportToExcelTriggered();

    // clang-format off
    ASSERT_EQ(excelFile.exists(), true);
    ASSERT_GE(excelFile.size(),   6317);
    // clang-format on
}

TEST_F(Test_OperationsTableWidget, Test_saveWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Time"),                            _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Name"),                            _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Description"),                     _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Price"),                           _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_AvgPriceFifo"),                    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_AvgPriceWavg"),                    _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Quantity"),                        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_RemainedQuantity"),                _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Payment"),                         _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Commission"),                      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_Yield"),                           _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_YieldWithCommission"),             _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_YieldWithCommissionPercent"),      _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_TotalYieldWithCommission"),        _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_TotalYieldWithCommissionPercent"), _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_RemainedMoney"),                   _));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("AAAAA/columnWidth_TotalMoney"),                      _));
    // clang-format on

    operationsTableWidget->saveWindowState("AAAAA");
}

TEST_F(Test_OperationsTableWidget, Test_loadWindowState)
{
    const InSequence seq;

    // clang-format off
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Time"),                            _)).WillOnce(Return(QVariant(110)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Name"),                            _)).WillOnce(Return(QVariant(82)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Description"),                     _)).WillOnce(Return(QVariant(309)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Price"),                           _)).WillOnce(Return(QVariant(61)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPriceFifo"),                    _)).WillOnce(Return(QVariant(142)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_AvgPriceWavg"),                    _)).WillOnce(Return(QVariant(153)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Quantity"),                        _)).WillOnce(Return(QVariant(95)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_RemainedQuantity"),                _)).WillOnce(Return(QVariant(167)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Payment"),                         _)).WillOnce(Return(QVariant(87)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Commission"),                      _)).WillOnce(Return(QVariant(85)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_Yield"),                           _)).WillOnce(Return(QVariant(69)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldWithCommission"),             _)).WillOnce(Return(QVariant(141)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_YieldWithCommissionPercent"),      _)).WillOnce(Return(QVariant(162)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_TotalYieldWithCommission"),        _)).WillOnce(Return(QVariant(107)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_TotalYieldWithCommissionPercent"), _)).WillOnce(Return(QVariant(128)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_RemainedMoney"),                   _)).WillOnce(Return(QVariant(107)));
    EXPECT_CALL(*settingsEditorMock, value(QString("AAAAA/columnWidth_TotalMoney"),                      _)).WillOnce(Return(QVariant(95)));
    // clang-format on

    operationsTableWidget->loadWindowState("AAAAA");
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
