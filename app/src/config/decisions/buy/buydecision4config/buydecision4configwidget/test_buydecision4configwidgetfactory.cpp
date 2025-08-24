#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/buydecision4configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/buy/buydecision4config/ibuydecision4config_mock.h"



using ::testing::StrictMock;



class Test_BuyDecision4ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new BuyDecision4ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    BuyDecision4ConfigWidgetFactory* factory;
};



TEST_F(Test_BuyDecision4ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_BuyDecision4ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<BuyDecision4ConfigMock> buyDecision4ConfigMock;

    IBuyDecision4ConfigWidget* widget = factory->newInstance(&buyDecision4ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
