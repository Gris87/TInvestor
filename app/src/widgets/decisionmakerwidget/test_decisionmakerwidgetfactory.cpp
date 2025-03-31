#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_DecisionMakerWidgetFactory, Test_constructor_and_destructor)
{
    DecisionMakerWidgetFactory factory;
}

TEST(Test_DecisionMakerWidgetFactory, Test_newInstance)
{
    StrictMock<SettingsEditorMock> settingsEditorMock;

    DecisionMakerWidgetFactory factory;

    IDecisionMakerWidget* widget = factory.newInstance(&settingsEditorMock, "Simulator", nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
