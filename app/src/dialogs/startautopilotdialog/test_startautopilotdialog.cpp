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
        InSequence seq;

        userStorageMock     = new StrictMock<UserStorageMock>();
        messageBoxUtilsMock = new StrictMock<MessageBoxUtilsMock>();
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        QMutex mutex;

        QList<Account> accounts;

        Account account1;
        Account account2;

        account1.setId("aaaa");
        account2.setId("bbbb");

        account1.name = "Babushka";
        account2.name = "Matreshka";

        accounts << account1 << account2;

        EXPECT_CALL(*userStorageMock, getMutex()).WillOnce(Return(&mutex));
        EXPECT_CALL(*userStorageMock, getAccounts()).WillOnce(ReturnRef(accounts));

        // clang-format off
        EXPECT_CALL(*settingsEditorMock, value(QString("StartAutoPilotDialog/account"),        QVariant(""))).WillOnce(Return(QVariant("")));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartAutoPilotDialog/follow"),         QVariant(false))).WillOnce(Return(QVariant(false)));
        EXPECT_CALL(*settingsEditorMock, value(QString("StartAutoPilotDialog/anotherAccount"), QVariant(""))).WillOnce(Return(QVariant("")));
        // clang-format on

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
