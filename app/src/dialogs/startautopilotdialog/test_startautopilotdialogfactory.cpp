#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"
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
    const InSequence seq;

    StartAutoPilotDialogFactory factory;

    StrictMock<UserStorageMock>     userStorageMock;
    StrictMock<MessageBoxUtilsMock> messageBoxUtilsMock;
    StrictMock<SettingsEditorMock>  settingsEditorMock;

    QMutex mutex;

    QList<Account> accounts;

    Account account1;
    Account account2;

    account1.setId("aaaa");
    account2.setId("bbbb");

    account1.name = "Babushka";
    account2.name = "Matreshka";

    accounts << account1 << account2;

    EXPECT_CALL(userStorageMock, getMutex()).WillOnce(Return(&mutex));
    EXPECT_CALL(userStorageMock, getAccounts()).WillOnce(ReturnRef(accounts));

    // clang-format off
    EXPECT_CALL(settingsEditorMock, value(QString("StartAutoPilotDialog/account"),        QVariant(""))).WillOnce(Return(QVariant("")));
    EXPECT_CALL(settingsEditorMock, value(QString("StartAutoPilotDialog/follow"),         QVariant(false))).WillOnce(Return(QVariant(false)));
    EXPECT_CALL(settingsEditorMock, value(QString("StartAutoPilotDialog/anotherAccount"), QVariant(""))).WillOnce(Return(QVariant("")));
    // clang-format on

    std::shared_ptr<IStartAutoPilotDialog> dialog =
        factory.newInstance(&userStorageMock, &messageBoxUtilsMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
