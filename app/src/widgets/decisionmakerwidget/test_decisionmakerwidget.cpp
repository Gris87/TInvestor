#include "src/widgets/decisionmakerwidget/decisionmakerwidget.h"
#include "ui_decisionmakerwidget.h"

#include <gtest/gtest.h>



class Test_DecisionMakerWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        decisionMakerWidget = new DecisionMakerWidget();
    }

    void TearDown()
    {
        delete decisionMakerWidget;
    }

    DecisionMakerWidget* decisionMakerWidget;
};



TEST_F(Test_DecisionMakerWidget, Test_constructor_and_destructor)
{
}
