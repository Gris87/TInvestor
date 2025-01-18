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
    int scheduleStartHour          = mConfig.getScheduleStartHour();
    int scheduleStartMinute        = mConfig.getScheduleStartMinute();
    int scheduleEndHour            = mConfig.getScheduleEndHour();
    int scheduleEndMinute          = mConfig.getScheduleEndMinute();
    int amountOfOperationsPerDay   = mConfig.getAmountOfOperationsPerDay();
    int amountOfOperationsPerStock = mConfig.getAmountOfOperationsPerStock();

    ui->autorunCheckBox->setChecked(mConfig.isAutorun());
    ui->refreshTimeoutSpinBox->setValue(mConfig.getRefreshTimeout());
    ui->useScheduleCheckBox->setChecked(mConfig.isUseSchedule());
    ui->scheduleStartTimeEdit->setTime(QTime(scheduleStartHour, scheduleStartMinute));
    ui->scheduleEndTimeEdit->setTime(QTime(scheduleEndHour, scheduleEndMinute));
    ui->limitOperationsPerDayCheckBox->setChecked(mConfig.isLimitOperationsPerDay());
    ui->amountOfOperationsPerDaySpinBox->setValue(amountOfOperationsPerDay);
    ui->limitOperationsPerStockCheckBox->setChecked(mConfig.isLimitOperationsPerStock());
    ui->amountOfOperationsPerStockSpinBox->setValue(amountOfOperationsPerStock);
    ui->simulatorConfigCommonCheckBox->setChecked(mConfig.isSimulatorConfigCommon());
    ui->autoPilotConfigCommonCheckBox->setChecked(mConfig.isAutoPilotConfigCommon());
}

void SettingsDialog::on_autorunCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setAutorun(checked);
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

void SettingsDialog::on_limitOperationsPerDayCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setLimitOperationsPerDay(checked);

    ui->amountOfOperationsPerDaySpinBox->setEnabled(checked);
}

void SettingsDialog::on_amountOfOperationsPerDaySpinBox_valueChanged(int value)
{
    if (value < ui->amountOfOperationsPerStockSpinBox->value())
    {
        ui->amountOfOperationsPerStockSpinBox->setValue(value);
    }

    mConfig.setAmountOfOperationsPerDay(value);
}

void SettingsDialog::on_limitOperationsPerStockCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setLimitOperationsPerStock(checked);

    ui->amountOfOperationsPerStockSpinBox->setEnabled(checked);
}

void SettingsDialog::on_amountOfOperationsPerStockSpinBox_valueChanged(int value)
{
    if (value > ui->amountOfOperationsPerDaySpinBox->value())
    {
        ui->amountOfOperationsPerDaySpinBox->setValue(value);
    }

    mConfig.setAmountOfOperationsPerStock(value);
}

void SettingsDialog::on_simulatorConfigCommonCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setSimulatorConfigCommon(checked);
}

void SettingsDialog::on_autoPilotConfigCommonCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig.setAutoPilotConfigCommon(checked);
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
