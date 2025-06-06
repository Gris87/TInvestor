#include "src/utils/settingseditor/settingseditor.h"

#include <gtest/gtest.h>



// NOLINTBEGIN(readability-magic-numbers)
TEST(Test_SettingsEditor, Test_constructor_and_destructor)
{
    const SettingsEditor editor("GrisCom", "TInvestor_Test");
}

TEST(Test_SettingsEditor, Test_setValue_and_value)
{
    SettingsEditor editor("GrisCom", "TInvestor_Test");

    editor.setValue("TestKey", 123);

    ASSERT_EQ(editor.value("TestKey", 44).toInt(), 123);
}

TEST(Test_SettingsEditor, Test_remove)
{
    SettingsEditor editor("GrisCom", "TInvestor_Test");

    editor.setValue("TestKey", 123);

    ASSERT_EQ(editor.value("TestKey", 44).toInt(), 123);

    editor.remove("TestKey");

    ASSERT_EQ(editor.value("TestKey", 44).toInt(), 44);
}
// NOLINTEND(readability-magic-numbers)
