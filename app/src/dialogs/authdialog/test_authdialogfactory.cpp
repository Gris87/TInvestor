#include "src/dialogs/authdialog/authdialogfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"



using ::testing::StrictMock;



TEST(Test_AuthDialogFactory, Test_constructor_and_destructor)
{
    const AuthDialogFactory factory;
}

TEST(Test_AuthDialogFactory, Test_newInstance)
{
    const AuthDialogFactory factory;

    StrictMock<UserStorageMock>     userStorageMock;
    StrictMock<MessageBoxUtilsMock> messageBoxUtilsMock;

    const std::shared_ptr<IAuthDialog> dialog = factory.newInstance(&userStorageMock, &messageBoxUtilsMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
