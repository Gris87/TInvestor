#include "src/widgets/accountchartwidget/accountchartwidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



TEST(Test_AccountChartWidgetFactory, Test_constructor_and_destructor)
{
    const AccountChartWidgetFactory factory;
}

TEST(Test_AccountChartWidgetFactory, Test_newInstance)
{
    const AccountChartWidgetFactory factory;

    StrictMock<FileDialogFactoryMock> fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>    settingsEditorMock;

    const IAccountChartWidget* widget = factory.newInstance(&fileDialogFactoryMock, &settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
