#pragma once



#include "src/dialogs/settingsdialog/isettingsdialog.h"

#include <gmock/gmock.h>



class SettingsDialogMock : public ISettingsDialog
{
    Q_OBJECT

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
