#include "src/dialogs/authdialog/authdialogfactory.h"

#include <gtest/gtest.h>



TEST(Test_AuthDialogFactory, Test_constructor_and_destructor)
{
    AuthDialogFactory factory;
}

TEST(Test_AuthDialogFactory, Test_newInstance)
{
    AuthDialogFactory factory;

    IAuthDialog* dialog = factory.newInstance(nullptr);

    ASSERT_TRUE(dialog != nullptr);

    delete dialog;
}
