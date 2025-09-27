#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/buydecision5configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision5config/ibuydecision5config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision5ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision5ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision5ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision5ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision5ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision5ConfigMock> buyDecision5ConfigMock;

    IBuyDecision5ConfigWidget* widget = factory->newInstance(&buyDecision5ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
