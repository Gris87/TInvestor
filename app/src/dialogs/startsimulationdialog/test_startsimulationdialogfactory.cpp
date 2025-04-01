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
    StrictMock<SettingsEditorMock> settingsEditorMock;

    StartSimulationDialogFactory factory;

    std::shared_ptr<IStartSimulationDialog> dialog = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
