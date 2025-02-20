#include "src/dialogs/authdialog/authdialogfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessagebox_mock.h"



using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_AuthDialogFactory, Test_constructor_and_destructor)
{
    AuthDialogFactory factory;
}

TEST(Test_AuthDialogFactory, Test_newInstance)
{
    AuthDialogFactory factory;

    StrictMock<UserStorageMock>     userStorageMock;
    StrictMock<MessageBoxUtilsMock> messageBoxUtilsMock;

    std::shared_ptr<IAuthDialog> dialog = factory.newInstance(&userStorageMock, &messageBoxUtilsMock, nullptr);

    ASSERT_TRUE(dialog != nullptr);
}
