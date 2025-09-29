#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/buydecision6configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision6config/ibuydecision6config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision6ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision6ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision6ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision6ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision6ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision6ConfigMock> buyDecision6ConfigMock;

    IBuyDecision6ConfigWidget* widget = factory->newInstance(&buyDecision6ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
