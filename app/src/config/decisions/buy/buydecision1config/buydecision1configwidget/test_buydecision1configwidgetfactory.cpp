#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/buydecision1configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision1ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision1ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision1ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision1ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision1ConfigMock> buyDecision1ConfigMock;

    IBuyDecision1ConfigWidget* widget = factory->newInstance(&buyDecision1ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
