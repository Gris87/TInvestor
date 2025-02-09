#include "src/config/settingseditor/settingseditor.h"

#include <QDebug>



SettingsEditor::SettingsEditor(const QString& organization, const QString& application) :
    ISettingsEditor(),
    mSettings(new QSettings(organization, application))
{
    qDebug() << "Create SettingsEditor";
}

SettingsEditor::~SettingsEditor()
{
    qDebug() << "Destroy SettingsEditor";

    delete mSettings;
}

void SettingsEditor::setValue(const QString& key, const QVariant& value)
{
    mSettings->setValue(key, value);
}

QVariant SettingsEditor::value(const QString& key, const QVariant& defaultValue)
{
    return mSettings->value(key, defaultValue);
}
