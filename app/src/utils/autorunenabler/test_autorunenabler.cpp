#include "src/utils/autorunenabler/autorunenabler.h"

#include <gtest/gtest.h>

#include "src/utils/settingseditor/isettingseditor_mock.h"



using ::testing::StrictMock;



class Test_AutorunEnabler : public ::testing::Test
{
protected:
    void SetUp() override
    {
        settingsEditorMock = new StrictMock<SettingsEditorMock>();

        enabler = new AutorunEnabler(settingsEditorMock);
    }

    void TearDown() override
    {
        delete enabler;
        delete settingsEditorMock;
    }

    AutorunEnabler*                 enabler;
    StrictMock<SettingsEditorMock>* settingsEditorMock;
};



TEST_F(Test_AutorunEnabler, Test_constructor_and_destructor)
{
}
