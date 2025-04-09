#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
class Test_StartSimulationDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        const InSequence seq;

        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("StartSimulationDialog/startMoney"), QVariant(100000))).WillOnce(Return(QVariant(100000)));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartSimulationDialog/dateRange"),  QVariant(false))).WillOnce(Return(QVariant(false)));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartSimulationDialog/fromDate"),   QVariant("2024-01-01"))).WillOnce(Return(QVariant("2024-01-01")));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartSimulationDialog/toDate"),     QVariant("2025-01-01"))).WillOnce(Return(QVariant("2025-01-01")));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartSimulationDialog/bestConfig"), QVariant(false))).WillOnce(Return(QVariant(false)));
        // clang-format on

        dialog = new StartSimulationDialog(settingsEditorMock);
    }

    void TearDown() override
    {
        delete dialog;
        delete settingsEditorMock;
    }

    StartSimulationDialog*          dialog;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_StartSimulationDialog, Test_constructor_and_destructor)
{
}

TEST_F(Test_StartSimulationDialog, Test_on_dateRangeRadioButton_toggled)
{
    ASSERT_EQ(dialog->ui->dateRangeWidget->isEnabled(), false);

    dialog->ui->dateRangeRadioButton->setChecked(true);
    ASSERT_EQ(dialog->ui->dateRangeWidget->isEnabled(), true);

    dialog->ui->realTimeRadioButton->setChecked(true);
    ASSERT_EQ(dialog->ui->dateRangeWidget->isEnabled(), false);
}

TEST_F(Test_StartSimulationDialog, Test_on_fromDateEdit_dateChanged)
{
    const QDate fromDate(2024, 1, 1);
    const QDate toDate(2025, 1, 1);
    const QDate newDate(2026, 1, 1);

    dialog->ui->fromDateEdit->setDate(fromDate);
    dialog->ui->toDateEdit->setDate(toDate);

    // clang-format off
    ASSERT_EQ(dialog->ui->fromDateEdit->date(), fromDate);
    ASSERT_EQ(dialog->ui->toDateEdit->date(),   toDate);
    // clang-format on

    dialog->ui->fromDateEdit->setDate(newDate);

    // clang-format off
    ASSERT_EQ(dialog->ui->fromDateEdit->date(), newDate);
    ASSERT_EQ(dialog->ui->toDateEdit->date(),   newDate);
    // clang-format on
}

TEST_F(Test_StartSimulationDialog, Test_on_toDateEdit_dateChanged)
{
    const QDate fromDate(2024, 1, 1);
    const QDate toDate(2025, 1, 1);
    const QDate newDate(2023, 1, 1);

    dialog->ui->fromDateEdit->setDate(fromDate);
    dialog->ui->toDateEdit->setDate(toDate);

    // clang-format off
    ASSERT_EQ(dialog->ui->fromDateEdit->date(), fromDate);
    ASSERT_EQ(dialog->ui->toDateEdit->date(),   toDate);
    // clang-format on

    dialog->ui->toDateEdit->setDate(newDate);

    // clang-format off
    ASSERT_EQ(dialog->ui->fromDateEdit->date(), newDate);
    ASSERT_EQ(dialog->ui->toDateEdit->date(),   newDate);
    // clang-format on
}

TEST_F(Test_StartSimulationDialog, Test_on_startButton_clicked)
{
    // clang-format off
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartSimulationDialog/startMoney"), QVariant(100000)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartSimulationDialog/dateRange"),  QVariant(false)));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartSimulationDialog/fromDate"),   QVariant("2024-01-01")));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartSimulationDialog/toDate"),     QVariant("2025-01-01")));
    EXPECT_CALL(*settingsEditorMock, setValue(QString("StartSimulationDialog/bestConfig"), QVariant(false)));
    // clang-format on

    dialog->ui->startButton->click();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init, readability-magic-numbers)
