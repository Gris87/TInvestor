#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_StartSimulationDialog : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        dialog = new StartSimulationDialog(settingsEditorMock);
    }

    void TearDown()
    {
        delete dialog;
        delete settingsEditorMock;
    }

    StartSimulationDialog* dialog;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_StartSimulationDialog, Test_constructor_and_destructor)
{
}
