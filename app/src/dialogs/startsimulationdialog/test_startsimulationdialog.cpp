#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_StartSimulationDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        InSequence seq;

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

    void TearDown()
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
