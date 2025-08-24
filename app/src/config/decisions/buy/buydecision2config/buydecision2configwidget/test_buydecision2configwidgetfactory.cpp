#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/buydecision2configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision2config/ibuydecision2config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision2ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision2ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision2ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision2ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision2ConfigMock> buyDecision2ConfigMock;

    IBuyDecision2ConfigWidget* widget = factory->newInstance(&buyDecision2ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
