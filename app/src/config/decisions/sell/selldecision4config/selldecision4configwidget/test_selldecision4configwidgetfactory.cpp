#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/selldecision4configwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/config/decisions/sell/selldecision4config/iselldecision4config_mock.h"



using ::testing::StrictMock;



class Test_SellDecision4ConfigWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new SellDecision4ConfigWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    SellDecision4ConfigWidgetFactory* factory;
};



TEST_F(Test_SellDecision4ConfigWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_SellDecision4ConfigWidgetFactory, Test_newInstance)
{
    StrictMock<SellDecision4ConfigMock> sellDecision4ConfigMock;

    ISellDecision4ConfigWidget* widget = factory->newInstance(&sellDecision4ConfigMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
