#pragma once



#include "src/dialogs/settingsdialog/isettingsdialogfactory.h"

#include <gmock/gmock.h>



class SettingsDialogFactoryMock : public ISettingsDialogFactory
{
public:
    SettingsDialogFactoryMock() :
        ISettingsDialogFactory()
    {
    }
    ~SettingsDialogFactoryMock() override = default;

    SettingsDialogFactoryMock(const SettingsDialogFactoryMock& another)            = delete;
    SettingsDialogFactoryMock& operator=(const SettingsDialogFactoryMock& another) = delete;

    MOCK_METHOD(std::shared_ptr<ISettingsDialog>, newInstance, (IConfig * config, QWidget* parent), (const, override));
};
