#pragma once



#include "src/utils/settingseditor/isettingseditor.h"

#include <QSettings>



class SettingsEditor : public ISettingsEditor
{
public:
    explicit SettingsEditor(const QString& organization, const QString& application);
    ~SettingsEditor();

    SettingsEditor(const SettingsEditor& another)            = delete;
    SettingsEditor& operator=(const SettingsEditor& another) = delete;

    void     setValue(const QString& key, const QVariant& value) override;
    QVariant value(const QString& key, const QVariant& defaultValue) override;

private:
    QSettings* mSettings;
};
