#include "src/utils/settingseditor/settingseditor.h"

#include <QCoreApplication>
#include <QDir>
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

TEST(Test_SettingsEditor, Test_ini_format)
{
    const QString appDir = qApp->applicationDirPath();

    ASSERT_TRUE(QDir(appDir + "/test/dir_for_settings").removeRecursively());
    ASSERT_TRUE(QDir().mkpath(appDir + "/test/dir_for_settings"));

    SettingsEditor* editor = new SettingsEditor(appDir + "/test/dir_for_settings/settings.ini", QSettings::IniFormat);
    editor->setValue("TestKey", 123);
    ASSERT_EQ(editor->value("TestKey", 44).toInt(), 123);

    delete editor;

    QFile qFile(appDir + "/test/dir_for_settings/settings.ini");
    ASSERT_TRUE(qFile.open(QIODevice::ReadOnly));
    QString content = QString::fromUtf8(qFile.readAll()).replace("\r\n", "\n");
    qFile.close();

    ASSERT_EQ(content, "[General]\nTestKey=123\n");

    editor = new SettingsEditor(appDir + "/test/dir_for_settings/settings.ini", QSettings::IniFormat);
    editor->remove("TestKey");
    ASSERT_EQ(editor->value("TestKey", 44).toInt(), 44);

    delete editor;

    ASSERT_TRUE(qFile.open(QIODevice::ReadOnly));
    content = QString::fromUtf8(qFile.readAll()).replace("\r\n", "\n");
    qFile.close();

    ASSERT_EQ(content, "");

    ASSERT_TRUE(QDir(appDir + "/test/dir_for_settings").removeRecursively());
}
// NOLINTEND(readability-magic-numbers)
