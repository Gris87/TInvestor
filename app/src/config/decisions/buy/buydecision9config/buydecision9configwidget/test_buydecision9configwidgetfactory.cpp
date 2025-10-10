#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/buydecision9configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision9config/ibuydecision9config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision9ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision9ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision9ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision9ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision9ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision9ConfigMock> buyDecision9ConfigMock;

    IBuyDecision9ConfigWidget* widget = factory->newInstance(&buyDecision9ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
