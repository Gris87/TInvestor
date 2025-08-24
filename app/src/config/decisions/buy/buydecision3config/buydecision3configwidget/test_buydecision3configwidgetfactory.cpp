#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/buydecision3configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision3ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision3ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision3ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision3ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision3ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision3ConfigMock> buyDecision3ConfigMock;

    IBuyDecision3ConfigWidget* widget = factory->newInstance(&buyDecision3ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
