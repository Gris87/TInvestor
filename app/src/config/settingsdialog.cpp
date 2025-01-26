#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>



const int simulationTabIndex = 1;
const int autoPilotTabIndex  = 2;



SettingsDialog::SettingsDialog(IConfig *config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog),
    mConfig(config)
{
    qDebug() << "Create SettingsDialog";

    ui->setupUi(this);

    mConfig->assign(config);
    ui->simulatorConfigWidget->setDecisionMakerConfig(mConfig->getSimulatorConfig());
    ui->autoPilotConfigWidget->setDecisionMakerConfig(mConfig->getAutoPilotConfig());

    updateUiFromConfig();
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "Destroy SettingsDialog";

    delete ui;
}

void SettingsDialog::updateUiFromConfig()
{
    int scheduleStartHour         = mConfig->getScheduleStartHour();
    int scheduleStartMinute       = mConfig->getScheduleStartMinute();
    int scheduleEndHour           = mConfig->getScheduleEndHour();
    int scheduleEndMinute         = mConfig->getScheduleEndMinute();
    int amountOfPurchasesPerDay   = mConfig->getAmountOfPurchasesPerDay();
    int amountOfPurchasesPerStock = mConfig->getAmountOfPurchasesPerStock();

    ui->autorunCheckBox->setChecked(mConfig->isAutorun());
    ui->refreshTimeoutSpinBox->setValue(mConfig->getRefreshTimeout());
    ui->useScheduleCheckBox->setChecked(mConfig->isUseSchedule());
    ui->scheduleStartTimeEdit->setTime(QTime(scheduleStartHour, scheduleStartMinute));
    ui->scheduleEndTimeEdit->setTime(QTime(scheduleEndHour, scheduleEndMinute));
    ui->limitPurchasesPerDayCheckBox->setChecked(mConfig->isLimitPurchasesPerDay());
    ui->amountOfPurchasesPerDaySpinBox->setValue(amountOfPurchasesPerDay);
    ui->limitPurchasesPerStockCheckBox->setChecked(mConfig->isLimitPurchasesPerStock());
    ui->amountOfPurchasesPerStockSpinBox->setValue(amountOfPurchasesPerStock);
    ui->commissionDoubleSpinBox->setValue(mConfig->getCommission());
    ui->limitStockPurchaseCheckBox->setChecked(mConfig->isLimitStockPurchase());
    ui->amountOfStockPurchaseSpinBox->setValue(mConfig->getAmountOfStockPurchase());
    ui->storageMonthLimitSpinBox->setValue(mConfig->getStorageMonthLimit());
    ui->simulatorConfigCommonCheckBox->setChecked(mConfig->isSimulatorConfigCommon());
    ui->autoPilotConfigCommonCheckBox->setChecked(mConfig->isAutoPilotConfigCommon());

    ui->simulatorConfigWidget->updateUiFromConfig();
    ui->autoPilotConfigWidget->updateUiFromConfig();
}

void SettingsDialog::on_autorunCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setAutorun(checked);
}

void SettingsDialog::on_refreshTimeoutSpinBox_valueChanged(int value)
{
    mConfig->setRefreshTimeout(value);
}

void SettingsDialog::on_useScheduleCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setUseSchedule(checked);

    ui->scheduleStartTimeEdit->setEnabled(checked);
    ui->scheduleEndTimeEdit->setEnabled(checked);
}

void SettingsDialog::on_scheduleStartTimeEdit_timeChanged(const QTime &time)
{
    if (time > ui->scheduleEndTimeEdit->time())
    {
        ui->scheduleEndTimeEdit->setTime(time);
    }

    mConfig->setScheduleStartHour(time.hour());
    mConfig->setScheduleStartMinute(time.minute());
}

void SettingsDialog::on_scheduleEndTimeEdit_timeChanged(const QTime &time)
{
    if (time < ui->scheduleStartTimeEdit->time())
    {
        ui->scheduleStartTimeEdit->setTime(time);
    }

    mConfig->setScheduleEndHour(time.hour());
    mConfig->setScheduleEndMinute(time.minute());
}

void SettingsDialog::on_limitPurchasesPerDayCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setLimitPurchasesPerDay(checked);

    ui->amountOfPurchasesPerDaySpinBox->setEnabled(checked);
}

void SettingsDialog::on_amountOfPurchasesPerDaySpinBox_valueChanged(int value)
{
    if (value < ui->amountOfPurchasesPerStockSpinBox->value())
    {
        ui->amountOfPurchasesPerStockSpinBox->setValue(value);
    }

    mConfig->setAmountOfPurchasesPerDay(value);
}

void SettingsDialog::on_limitPurchasesPerStockCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setLimitPurchasesPerStock(checked);

    ui->amountOfPurchasesPerStockSpinBox->setEnabled(checked);
}

void SettingsDialog::on_amountOfPurchasesPerStockSpinBox_valueChanged(int value)
{
    if (value > ui->amountOfPurchasesPerDaySpinBox->value())
    {
        ui->amountOfPurchasesPerDaySpinBox->setValue(value);
    }

    mConfig->setAmountOfPurchasesPerStock(value);
}

void SettingsDialog::on_commissionDoubleSpinBox_valueChanged(double value)
{
    mConfig->setCommission(value);
}

void SettingsDialog::on_limitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setLimitStockPurchase(checked);

    ui->amountOfStockPurchaseSpinBox->setEnabled(checked);
}

void SettingsDialog::on_amountOfStockPurchaseSpinBox_valueChanged(int value)
{
    mConfig->setAmountOfStockPurchase(value);
}

void SettingsDialog::on_simulatorConfigCommonCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setSimulatorConfigCommon(checked);

    if (checked)
    {
        ui->mainTabWidget->setTabText(simulationTabIndex, tr("Decision maker"));
        ui->mainTabWidget->removeTab(autoPilotTabIndex);
    }
    else
    {
        ui->mainTabWidget->insertTab(autoPilotTabIndex, ui->autoPilotTab, tr("Auto-pilot"));
        ui->mainTabWidget->setTabText(simulationTabIndex, tr("Simulation"));
    }
}

void SettingsDialog::on_autoPilotConfigCommonCheckBox_checkStateChanged(const Qt::CheckState &value)
{
    bool checked = value == Qt::Checked;

    mConfig->setAutoPilotConfigCommon(checked);

    if (checked)
    {
        ui->mainTabWidget->setTabText(autoPilotTabIndex, tr("Decision maker"));
        ui->mainTabWidget->removeTab(simulationTabIndex);
    }
    else
    {
        ui->mainTabWidget->insertTab(simulationTabIndex, ui->simulationTab, tr("Simulation"));
        ui->mainTabWidget->setTabText(autoPilotTabIndex, tr("Auto-pilot"));
    }
}

void SettingsDialog::on_storageMonthLimitSpinBox_valueChanged(int value)
{
    mConfig->setStorageMonthLimit(value);
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
    mConfig->makeDefault();
    updateUiFromConfig();
}
