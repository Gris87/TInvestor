#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_DecisionMakerWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        decisionMakerWidget = new DecisionMakerWidget(settingsEditorMock, "Simulator");
    }

    void TearDown()
    {
        delete decisionMakerWidget;
        delete settingsEditorMock;
    }

    DecisionMakerWidget*            decisionMakerWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_DecisionMakerWidget, Test_constructor_and_destructor)
{
}
