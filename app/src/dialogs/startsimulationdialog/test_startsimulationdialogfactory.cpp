#include "src/dialogs/startsimulationdialog/startsimulationdialogfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_StartSimulationDialogFactory, Test_constructor_and_destructor)
{
    const StartSimulationDialogFactory factory;
}

TEST(Test_StartSimulationDialogFactory, Test_newInstance)
{
    const InSequence seq;

    const StartSimulationDialogFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/mode"),       QVariant("REALTIME"))).WillOnce(Return(QVariant("REALTIME")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/startMoney"), QVariant(100000))).WillOnce(Return(QVariant(100000)));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/fromDate"),   QVariant("2024-01-01"))).WillOnce(Return(QVariant("2024-01-01")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/toDate"),     QVariant("2025-01-01"))).WillOnce(Return(QVariant("2025-01-01")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartSimulationDialog/bestConfig"), QVariant(false))).WillOnce(Return(QVariant(false)));
    // clang-format on

    const std::shared_ptr<IStartSimulationDialog> dialog = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
// NOLINTEND(readability-magic-numbers)
