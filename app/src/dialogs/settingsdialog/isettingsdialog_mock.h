#pragma once



#include "src/config/settingsdialog/isettingsdialog.h"

#include <gmock/gmock.h>



class SettingsDialogMock : public ISettingsDialog
{
public:
    explicit SettingsDialogMock(QWidget* parent = nullptr) :
        ISettingsDialog(parent)
    {
    }
    ~SettingsDialogMock() override = default;

    SettingsDialogMock(const SettingsDialogMock& another)            = delete;
    SettingsDialogMock& operator=(const SettingsDialogMock& another) = delete;

    MOCK_METHOD(void, updateUiFromConfig, (), (override));
    MOCK_METHOD(int, exec, (), (override));
};
