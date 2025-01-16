#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>



SettingsDialog::SettingsDialog(const Config &config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    mConfig(config)
{
    qDebug() << "Create SettingsDialog";

    ui->setupUi(this);

    updateUiFromConfig();
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "Destroy SettingsDialog";

    delete ui;
}

const Config& SettingsDialog::getConfig()
{
    return mConfig;
}

void SettingsDialog::updateUiFromConfig()
{
    int scheduleStartHour   = mConfig.getScheduleStartHour();
    int scheduleStartMinute = mConfig.getScheduleStartMinute();
    int scheduleEndHour     = mConfig.getScheduleEndHour();
    int scheduleEndMinute   = mConfig.getScheduleEndMinute();

    ui->refreshTimeoutSpinBox->setValue(mConfig.getRefreshTimeout());
    ui->useScheduleCheckBox->setChecked(mConfig.isUseSchedule());
    ui->scheduleStartTimeEdit->setTime(QTime(scheduleStartHour, scheduleStartMinute));
    ui->scheduleEndTimeEdit->setTime(QTime(scheduleEndHour, scheduleEndMinute));
    ui->useForSimulatorAndAutoPilotCheckBox->setChecked(mConfig.isUseForSimulatorAndAutoPilot());
}

void SettingsDialog::on_refreshTimeoutSpinBox_valueChanged(int value)
{
    mConfig.setRefreshTimeout(value);
}

void SettingsDialog::on_useScheduleCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setUseSchedule(checked);

    ui->scheduleStartTimeEdit->setEnabled(checked);
    ui->scheduleEndTimeEdit->setEnabled(checked);
}

void SettingsDialog::on_scheduleStartTimeEdit_timeChanged(const QTime &time)
{
    if (time > ui->scheduleEndTimeEdit->time())
    {
        ui->scheduleEndTimeEdit->setTime(time);
    }

    mConfig.setScheduleStartHour(time.hour());
    mConfig.setScheduleStartMinute(time.minute());
}

void SettingsDialog::on_scheduleEndTimeEdit_timeChanged(const QTime &time)
{
    if (time < ui->scheduleStartTimeEdit->time())
    {
        ui->scheduleStartTimeEdit->setTime(time);
    }

    mConfig.setScheduleEndHour(time.hour());
    mConfig.setScheduleEndMinute(time.minute());
}

void SettingsDialog::on_useForSimulatorAndAutoPilotCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setUseForSimulatorAndAutoPilot(checked);
}

void SettingsDialog::on_okButton_clicked()
{
    accept();
}

void SettingsDialog::on_cancelButton_clicked()
{
    reject();
}

void SettingsDialog::on_defaultButton_clicked()
{
    mConfig.makeDefault();
    updateUiFromConfig();
}

