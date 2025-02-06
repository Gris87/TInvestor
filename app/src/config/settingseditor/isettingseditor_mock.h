#pragma once



#include "src/config/settingseditor/isettingseditor.h"

#include <gmock/gmock.h>



class SettingsEditorMock : public ISettingsEditor
{
public:
    SettingsEditorMock() : ISettingsEditor() {}
    ~SettingsEditorMock() override = default;

    SettingsEditorMock(const SettingsEditorMock &another) = delete;
    SettingsEditorMock& operator=(const SettingsEditorMock &another) = delete;

    MOCK_METHOD2(setValue, void(const QString &key, const QVariant &value));
    MOCK_METHOD2(value, QVariant(const QString &key, const QVariant &defaultValue));
};
