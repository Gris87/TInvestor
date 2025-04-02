#include "src/widgets/operationstablewidget/operationstablewidget.h"
#include "ui_operationstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_OperationsTableWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        operationsTableWidget = new OperationsTableWidget(settingsEditorMock);
    }

    void TearDown()
    {
        delete operationsTableWidget;
        delete settingsEditorMock;
    }

    OperationsTableWidget*          operationsTableWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_OperationsTableWidget, Test_constructor_and_destructor)
{
}
