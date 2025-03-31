#include "src/dialogs/startautopilotdialog/startautopilotdialogfactory.h"

#include <gtest/gtest.h>



TEST(Test_StartAutoPilotDialogFactory, Test_constructor_and_destructor)
{
    StartAutoPilotDialogFactory factory;
}

TEST(Test_StartAutoPilotDialogFactory, Test_newInstance)
{
    StartAutoPilotDialogFactory factory;

    std::shared_ptr<IStartAutoPilotDialog> dialog = factory.newInstance(nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
