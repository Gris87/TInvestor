#include "src/widgets/decisionmakerwidget/decisionmakerwidgetfactory.h"

#include <gtest/gtest.h>



TEST(Test_DecisionMakerWidgetFactory, Test_constructor_and_destructor)
{
    DecisionMakerWidgetFactory factory;
}

TEST(Test_DecisionMakerWidgetFactory, Test_newInstance)
{
    DecisionMakerWidgetFactory factory;

    IDecisionMakerWidget* widget = factory.newInstance(nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
