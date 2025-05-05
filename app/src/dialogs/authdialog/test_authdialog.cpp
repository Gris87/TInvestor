#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <gtest/gtest.h>

#include "src/domain/user/user.h"
#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"



using ::testing::InSequence;
using ::testing::Return;
using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AuthDialog : public ::testing::Test
{
protected:
    void SetUp() override
    {
        userStorageMock     = new StrictMock<UserStorageMock>();
        messageBoxUtilsMock = new StrictMock<MessageBoxUtilsMock>();

        dialog = new AuthDialog(userStorageMock, messageBoxUtilsMock);
    }

    void TearDown() override
    {
        delete dialog;
        delete userStorageMock;
        delete messageBoxUtilsMock;
    }

    AuthDialog*                      dialog;
    StrictMock<UserStorageMock>*     userStorageMock;
    StrictMock<MessageBoxUtilsMock>* messageBoxUtilsMock;
};



TEST_F(Test_AuthDialog, Test_constructor_and_destructor)
{
    ASSERT_EQ(dialog->ui->tokenLineEdit->text(), SANDBOX_TOKEN);
}

TEST_F(Test_AuthDialog, Test_getToken)
{
    ASSERT_EQ(dialog->getToken(), SANDBOX_TOKEN);
}

TEST_F(Test_AuthDialog, Test_on_loginButton_clicked)
{
    const InSequence seq;

    EXPECT_CALL(
        *messageBoxUtilsMock,
        warning(
            dialog,
            QString("Token is invalid"),
            QString("You should provide correct token"),
            QMessageBox::StandardButtons(QMessageBox::Ok),
            QMessageBox::NoButton
        )
    )
        .WillOnce(Return(QMessageBox::Ok));

    dialog->ui->tokenLineEdit->setText("BadRegexpToken");
    dialog->ui->loginButton->click();

    dialog->ui->tokenLineEdit->setText(SANDBOX_TOKEN);
    dialog->ui->loginButton->click();

    const QString niceToken = "t.aaaaaaaaaaaaaaaaaaaaa0000000000000AAAAAAAAAAAAAAAAAAA_AAAAAAAAAA-aaaa0000AAAAAAAAAAAAA";

    dialog->ui->tokenLineEdit->setText(niceToken);
    dialog->ui->loginButton->click();
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
