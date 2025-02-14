#pragma once



#include "src/config/settingseditor/isettingseditor.h"

#include <gmock/gmock.h>



class SettingsEditorMock : public ISettingsEditor
{
public:
    SettingsEditorMock() :
        ISettingsEditor()
    {
    }
    ~SettingsEditorMock() override = default;

    SettingsEditorMock(const SettingsEditorMock& another)            = delete;
    SettingsEditorMock& operator=(const SettingsEditorMock& another) = delete;

    MOCK_METHOD(void, setValue, (const QString& key, const QVariant& value), (override));
    MOCK_METHOD(QVariant, value, (const QString& key, const QVariant& defaultValue), (override));
};
