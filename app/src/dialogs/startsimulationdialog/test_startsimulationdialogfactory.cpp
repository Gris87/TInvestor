#include "src/dialogs/startsimulationdialog/startsimulationdialogfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_StartSimulationDialogFactory, Test_constructor_and_destructor)
{
    StartSimulationDialogFactory factory;
}

TEST(Test_StartSimulationDialogFactory, Test_newInstance)
{
    InSequence seq;

    StartSimulationDialogFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/startMoney"), QVariant(100000))).WillOnce(Return(QVariant(100000)));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/dateRange"),  QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/fromDate"),   QVariant("2024-01-01"))).WillOnce(Return(QVariant("2024-01-01")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/toDate"),     QVariant("2025-01-01"))).WillOnce(Return(QVariant("2025-01-01")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/bestConfig"), QVariant(false))).WillOnce(Return(QVariant(false)));
    // clang-format on

    std::shared_ptr<IStartSimulationDialog> dialog = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
