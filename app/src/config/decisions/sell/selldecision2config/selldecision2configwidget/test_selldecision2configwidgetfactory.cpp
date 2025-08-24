#include "src/config/decisions/sell/selldecision2config/selldecision2configwidget/selldecision2configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision2config/iselldecision2config_mock.h"



using ::testing::StrictMock;



class Test_SellDecision2ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new SellDecision2ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    SellDecision2ConfigWidgetFactory* factory;
};



TEST_F(Test_SellDecision2ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision2ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<SellDecision2ConfigMock> sellDecision2ConfigMock;

    ISellDecision2ConfigWidget* widget = factory->newInstance(&sellDecision2ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
