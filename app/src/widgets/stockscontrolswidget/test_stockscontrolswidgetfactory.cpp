#include "src/widgets/stockscontrolswidget/stockscontrolswidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



class Test_StocksControlsWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new StocksControlsWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    StocksControlsWidgetFactory* factory;
};



TEST_F(Test_StocksControlsWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_StocksControlsWidgetFactory, Test_newInstance)
{
    StrictMock<SettingsEditorMock> settingsEditorMock;

    const IStocksControlsWidget* widget = factory->newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
