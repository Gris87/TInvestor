#include "src/widgets/accountchartwidget/accountchartwidget.h"

#include <gtest/gtest.h>

#include "src/utils/filedialog/ifiledialogfactory_mock.h"
#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



// NOLINTBEGIN(cppcoreguidelines-pro-type-member-init)
class Test_AccountChartWidget : public ::testing::Test
{
protected:
    void SetUp() override
    {
        fileDialogFactoryMock = new StrictMock<FileDialogFactoryMock>();
        settingsEditorMock    = new StrictMock<SettingsEditorMock>();

        accountChartWidget = new AccountChartWidget(fileDialogFactoryMock, settingsEditorMock);
    }

    void TearDown() override
    {
        delete accountChartWidget;
        delete fileDialogFactoryMock;
        delete settingsEditorMock;
    }

    AccountChartWidget*                accountChartWidget;
    StrictMock<FileDialogFactoryMock>* fileDialogFactoryMock;
    StrictMock<SettingsEditorMock>*    settingsEditorMock;
};



TEST_F(Test_AccountChartWidget, Test_constructor_and_destructor)
{
}
// NOLINTEND(cppcoreguidelines-pro-type-member-init)
