#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/buydecision7configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision7config/ibuydecision7config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision7ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision7ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision7ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision7ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision7ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision7ConfigMock> buyDecision7ConfigMock;

    IBuyDecision7ConfigWidget* widget = factory->newInstance(&buyDecision7ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
