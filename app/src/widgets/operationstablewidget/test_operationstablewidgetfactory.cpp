#include "src/widgets/operationstablewidget/operationstablewidgetfactory.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



TEST(Test_OperationsTableWidgetFactory, Test_constructor_and_destructor)
{
    OperationsTableWidgetFactory factory;
}

TEST(Test_OperationsTableWidgetFactory, Test_newInstance)
{
    OperationsTableWidgetFactory factory;

    StrictMock<SettingsEditorMock> settingsEditorMock;

    IOperationsTableWidget* widget = factory.newInstance(&settingsEditorMock, nullptr);
    ASSERT_TRUE(widget != nullptr);

    delete widget;
}
