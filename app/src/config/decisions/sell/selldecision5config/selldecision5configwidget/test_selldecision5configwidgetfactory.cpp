#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/selldecision5configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision5config/iselldecision5config_mock.h"



using ::testing::StrictMock;



class Test_SellDecision5ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new SellDecision5ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    SellDecision5ConfigWidgetFactory* factory;
};



TEST_F(Test_SellDecision5ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision5ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<SellDecision5ConfigMock> sellDecision5ConfigMock;

    ISellDecision5ConfigWidget* widget = factory->newInstance(&sellDecision5ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
