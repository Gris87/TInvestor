#include "src/dialogs/authdialog/authdialogfactory.h"

#include <gtest/gtest.h>

#include "src/storage/user/iuserstorage_mock.h"
#include "src/utils/messagebox/imessageboxutils_mock.h"



using ::testing::StrictMock;



class Test_AuthDialogFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new AuthDialogFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    AuthDialogFactory* factory;
};



TEST_F(Test_AuthDialogFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_AuthDialogFactory, Test_newInstance)
{
    StrictMock<UserStorageMock>     userStorageMock;
    StrictMock<MessageBoxUtilsMock> messageBoxUtilsMock;

    const std::shared_ptr<IAuthDialog> dialog = factory->newInstance(&userStorageMock, &messageBoxUtilsMock, nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
