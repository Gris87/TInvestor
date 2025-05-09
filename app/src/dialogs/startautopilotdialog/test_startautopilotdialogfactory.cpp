#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_StartAutoPilotDialogFactory, Test_constructor_and_destructor)
{
    const StartAutoPilotDialogFactory factory;
}

TEST(Test_StartAutoPilotDialogFactory, Test_newInstance)
{
    const InSequence seq;

    const StartAutoPilotDialogFactory factory;

    StrictMock<UserStorageMock>     userStorageMock;
    StrictMock<MessageBoxUtilsMock> messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>  settingsEditorMock;

    QMutex mutex;

    Accounts accounts;

    Account account1;
    Account account2;

    account1.index = 0;
    account1.id    = "aaaa";
    account1.name  = "Babushka";

    account2.index = 1;
    account2.id    = "bbbb";
    account2.name  = "Matreshka";

    accounts[account1.hash()] = account1;
    accounts[account2.hash()] = account2;

    EXPECT_CALL(userStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(userStorageMock, getAccounts()).WillOnce(ReturnRef(accounts));

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("StartAutoPilotDialog/account"),        QVariant(""))).WillOnce(Return(QVariant("")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartAutoPilotDialog/follow"),         QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(settingsEditorMock, value(QString("StartAutoPilotDialog/anotherAccount"), QVariant(""))).WillOnce(Return(QVariant("")));
    // clang-format on

    const std::shared_ptr<IStartAutoPilotDialog> dialog =
        factory.newInstance(&userStorageMock, &messageBoxUtilsMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
