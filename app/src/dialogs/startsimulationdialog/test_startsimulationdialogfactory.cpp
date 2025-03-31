#include "src/dialogs/startsimulationdialog/startsimulationdialogfactory.h"

#include <gtest/gtest.h>



TEST(Test_StartSimulationDialogFactory, Test_constructor_and_destructor)
{
    StartSimulationDialogFactory factory;
}

TEST(Test_StartSimulationDialogFactory, Test_newInstance)
{
    StartSimulationDialogFactory factory;

    std::shared_ptr<IStartSimulationDialog> dialog = factory.newInstance(nullptr);
    ASSERT_TRUE(dialog != nullptr);
}
