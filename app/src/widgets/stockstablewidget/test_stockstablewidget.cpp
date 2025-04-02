#include "src/widgets/stockstablewidget/stockstablewidget.h"
#include "ui_stockstablewidget.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::InSequence;
using ::testing::NotNull;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::StrictMock;



class Test_StocksTableWidget : public ::testing::Test
{
protected:
    void SetUp()
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        stocksTableWidget = new StocksTableWidget(settingsEditorMock);
    }

    void TearDown()
    {
        delete stocksTableWidget;
        delete settingsEditorMock;
    }

    StocksTableWidget*              stocksTableWidget;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_StocksTableWidget, Test_constructor_and_destructor)
{
}
