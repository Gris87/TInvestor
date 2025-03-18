#include "src/utils/settingseditor/settingseditor.h"

#include <QDebug>



SettingsEditor::SettingsEditor(const QString& organization, const QString& application) :
    ISettingsEditor(),
    mSettings(QSettings(organization, application))
{
    qDebug() << "Create SettingsEditor";
}

SettingsEditor::~SettingsEditor()
{
    qDebug() << "Destroy SettingsEditor";
}

void SettingsEditor::setValue(const QString& key, const QVariant& value)
{
    mSettings.setValue(key, value);
}

QVariant SettingsEditor::value(const QString& key, const QVariant& defaultValue)
{
    return mSettings.value(key, defaultValue);
}

void SettingsEditor::remove(const QString& key)
{
    mSettings.remove(key);
}
