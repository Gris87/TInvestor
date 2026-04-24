#pragma once



#include "src/dialogs/settingsdialog/isettingsdialog.h"

#include "src/config/iconfig.h"



namespace Ui
{
class SettingsDialog;
}



class SettingsDialog : public ISettingsDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(IConfig* config, QWidget* parent = nullptr);
    ~SettingsDialog() override;

    SettingsDialog(const SettingsDialog& another)            = delete;
    SettingsDialog& operator=(const SettingsDialog& another) = delete;

    void updateUiFromConfig() override;

    Ui::SettingsDialog* ui;

private:
    IConfig* mConfig;

private slots:
    void on_autorunCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_serverAddressLineEdit_textChanged(const QString& value);
    void on_serverPortLimitSpinBox_valueChanged(int value);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_defaultButton_clicked();
};
