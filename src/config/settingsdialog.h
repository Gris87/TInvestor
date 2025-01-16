#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include <QTime>

#include "src/config/config.h"



namespace Ui {
class SettingsDialog;
}



class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const Config &config, QWidget *parent = nullptr);
    ~SettingsDialog();

    const Config& getConfig();

private:
    void updateUiFromConfig();

    Ui::SettingsDialog *ui;

    Config mConfig;

private slots:
    void on_refreshTimeoutSpinBox_valueChanged(int value);
    void on_useScheduleCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_scheduleStartTimeEdit_timeChanged(const QTime &time);
    void on_scheduleEndTimeEdit_timeChanged(const QTime &time);
    void on_useForSimulatorAndAutoPilotCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_defaultButton_clicked();
};



#endif // SETTINGSDIALOG_H
