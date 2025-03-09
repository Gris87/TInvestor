#pragma once



#include <QVariant>



class ISettingsEditor
{
public:
    ISettingsEditor()
    {
    }
    virtual ~ISettingsEditor() = default;

    ISettingsEditor(const ISettingsEditor& another)            = delete;
    ISettingsEditor& operator=(const ISettingsEditor& another) = delete;

    virtual void     setValue(const QString& key, const QVariant& value)     = 0;
    virtual QVariant value(const QString& key, const QVariant& defaultValue) = 0;
    virtual void     remove(const QString& key)                              = 0;
};
