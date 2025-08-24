#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/selldecision1configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision1config/iselldecision1config_mock.h"



using ::testing::StrictMock;



class Test_SellDecision1ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new SellDecision1ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    SellDecision1ConfigWidgetFactory* factory;
};



TEST_F(Test_SellDecision1ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision1ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<SellDecision1ConfigMock> sellDecision1ConfigMock;

    ISellDecision1ConfigWidget* widget = factory->newInstance(&sellDecision1ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
