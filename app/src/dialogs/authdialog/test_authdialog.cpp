#include "src/dialogs/authdialog/authdialog.h"
#include "ui_authdialog.h"

#include <gtest/gtest.h>



class Test_AuthDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        dialog = new AuthDialog();
    }

    void TearDown()
    {
        delete dialog;
    }

    AuthDialog* dialog;
};



TEST_F(Test_AuthDialog, Test_constructor_and_destructor)
{
}
