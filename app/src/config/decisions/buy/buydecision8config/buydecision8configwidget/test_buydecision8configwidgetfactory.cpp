#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/buydecision8configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision8config/ibuydecision8config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision8ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision8ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision8ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision8ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision8ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision8ConfigMock> buyDecision8ConfigMock;

    IBuyDecision8ConfigWidget* widget = factory->newInstance(&buyDecision8ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
