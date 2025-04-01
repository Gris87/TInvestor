#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"
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
        userStorageMock    = new StrictMock<UserStorageMock>();
        messageBoxUtilsMock = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        dialog = new StartAutoPilotDialog(userStorageMock, messageBoxUtilsMock, settingsEditorMock);
    }

    void TearDown()
    {
        delete dialog;
        delete userStorageMock;
        delete messageBoxUtilsMock;
        delete settingsEditorMock;
    }

    StartAutoPilotDialog*           dialog;
    StrictMock<UserStorageMock>*    userStorageMock;
    StrictMock<MessageBoxUtilsMock>* messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_StartAutoPilotDialog, Test_constructor_and_destructor)
{
}
