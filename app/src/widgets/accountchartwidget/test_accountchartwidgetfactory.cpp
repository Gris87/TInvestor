#include "src/widgets/accountchartwidget/accountchartwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



class Test_AccountChartWidgetFactory : public ::testing::Test
{
protected:
    void SetUp() override
    {
        factory = new AccountChartWidgetFactory();
    }

    void TearDown() override
    {
        delete factory;
    }

    AccountChartWidgetFactory* factory;
};



TEST_F(Test_AccountChartWidgetFactory, Test_constructor_and_destructor)
{
}

TEST_F(Test_AccountChartWidgetFactory, Test_newInstance)
{
    StrictMock<FileDialogFactoryMock> fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>    settingsEditorMock;

    const IAccountChartWidget* widget = factory->newInstance(&fileDialogFactoryMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
