#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_AuthDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        userStorageMock = new StrictMock<UserStorageMock>();

        QString testToken = "TestToken";
        EXPECT_CALL(*userStorageMock, getToken()).WillOnce(ReturnRef(testToken));

        dialog = new AuthDialog(userStorageMock);
    }

    void TearDown()
    {
        delete dialog;
        delete userStorageMock;
    }

    AuthDialog*                  dialog;
    StrictMock<UserStorageMock>* userStorageMock;
};



TEST_F(Test_AuthDialog, Test_constructor_and_destructor)
{
    ASSERT_EQ(dialog->ui->tokenLineEdit->text(), "TestToken");
}

TEST_F(Test_AuthDialog, Test_on_loginButton_clicked)
{
    dialog->ui->tokenLineEdit->setText("");
    dialog->ui->loginButton->click();

    dialog->ui->tokenLineEdit->setText("NiceToken");
    dialog->ui->loginButton->click();
}
