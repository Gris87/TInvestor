#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_StartAutoPilotDialogFactory, Test_constructor_and_destructor)
{
    StartAutoPilotDialogFactory factory;
}

TEST(Test_StartAutoPilotDialogFactory, Test_newInstance)
{
    StrictMock<SettingsEditorMock> settingsEditorMock;

    StartAutoPilotDialogFactory factory;

    std::shared_ptr<IStartAutoPilotDialog> dialog = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
