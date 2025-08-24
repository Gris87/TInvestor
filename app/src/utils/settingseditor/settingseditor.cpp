#include "src/utils/settingseditor/settingseditor.h"

#include <QDebug>



SettingsEditor::SettingsEditor(const QString& organization, const QString& application) :
    ISettingsEditor(),
    mRwMutex(new QReadWriteLock()),
    mSettings(QSettings(organization, application))
{
    qDebug() << "Create SettingsEditor";
}

SettingsEditor::SettingsEditor(const QString& fileName, QSettings::Format format) :
    ISettingsEditor(),
    mRwMutex(new QReadWriteLock()),
    mSettings(QSettings(fileName, format))
{
    qDebug() << "Create SettingsEditor";
}

SettingsEditor::~SettingsEditor()
{
    qDebug() << "Destroy SettingsEditor";

    delete mRwMutex;
}

void SettingsEditor::setValue(const QString& key, const QVariant& value)
{
    const QWriteLocker lock(mRwMutex);

    mSettings.setValue(key, value);
}

QVariant SettingsEditor::value(const QString& key, const QVariant& defaultValue)
{
    const QReadLocker lock(mRwMutex);

    return mSettings.value(key, defaultValue);
}

void SettingsEditor::remove(const QString& key)
{
    const QWriteLocker lock(mRwMutex);

    mSettings.remove(key);
}
