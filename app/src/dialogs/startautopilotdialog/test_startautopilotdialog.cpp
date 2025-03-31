#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <gtest/gtest.h>



class Test_StartAutoPilotDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        dialog = new StartAutoPilotDialog();
    }

    void TearDown()
    {
        delete dialog;
    }

    StartAutoPilotDialog* dialog;
};



TEST_F(Test_StartAutoPilotDialog, Test_constructor_and_destructor)
{
}
