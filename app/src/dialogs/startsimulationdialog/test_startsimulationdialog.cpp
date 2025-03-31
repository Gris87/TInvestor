#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <gtest/gtest.h>



class Test_StartSimulationDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        dialog = new StartSimulationDialog();
    }

    void TearDown()
    {
        delete dialog;
    }

    StartSimulationDialog* dialog;
};



TEST_F(Test_StartSimulationDialog, Test_constructor_and_destructor)
{
}
