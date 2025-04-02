#include "src/widgets/portfoliotablewidget/portfoliotablewidget.h"
#include "ui_portfoliotablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_PortfolioTableWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        portfolioTableWidget = new PortfolioTableWidget(settingsEditorMock);
    }

    void TearDown()
    {
        delete portfolioTableWidget;
        delete settingsEditorMock;
    }

    PortfolioTableWidget*           portfolioTableWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_PortfolioTableWidget, Test_constructor_and_destructor)
{
}
