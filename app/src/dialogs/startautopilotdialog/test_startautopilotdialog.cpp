#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_StartAutoPilotDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        dialog = new StartAutoPilotDialog(settingsEditorMock);
    }

    void TearDown()
    {
        delete dialog;
        delete settingsEditorMock;
    }

    StartAutoPilotDialog* dialog;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_StartAutoPilotDialog, Test_constructor_and_destructor)
{
}
