#include "src/dialogs/settingsdialog/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>



const int SIMULATION_TAB_INDEX = 1;
const int AUTO_PILOT_TAB_INDEX = 2;



SettingsDialog::SettingsDialog(
    IConfig*                           config,
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    QWidget*                           parent
) :
    ISettingsDialog(parent),
    ui(new Ui::SettingsDialog),
    mConfig(config)
{
    qDebug() << "Create SettingsDialog";

    ui->setupUi(this);

    mSimulatorConfigWidget = decisionMakerConfigWidgetFactory->newInstance(
        mConfig->getSimulatorConfig(),
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        ui->simulationTab
    );
    mAutoPilotConfigWidget = decisionMakerConfigWidgetFactory->newInstance(
        mConfig->getAutoPilotConfig(),
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        ui->autoPilotTab
    );

    ui->layoutForSimulatorConfigWidget->addWidget(mSimulatorConfigWidget);
    ui->layoutForAutoPilotConfigWidget->addWidget(mAutoPilotConfigWidget);
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "Destroy SettingsDialog";

    delete ui;
}

void SettingsDialog::updateUiFromConfig()
{
    const int scheduleStartHour   = mConfig->getScheduleStartHour();
    const int scheduleStartMinute = mConfig->getScheduleStartMinute();
    const int scheduleEndHour     = mConfig->getScheduleEndHour();
    const int scheduleEndMinute   = mConfig->getScheduleEndMinute();

    mSimulatorConfigWidget->updateUiFromConfig();
    mAutoPilotConfigWidget->updateUiFromConfig();

    ui->autorunCheckBox->setChecked(mConfig->isAutorun());
    ui->makeDecisionTimeoutSpinBox->setValue(mConfig->getMakeDecisionTimeout());
    ui->useScheduleCheckBox->setChecked(mConfig->isUseSchedule());
    ui->scheduleStartTimeEdit->setTime(QTime(scheduleStartHour, scheduleStartMinute));
    ui->scheduleEndTimeEdit->setTime(QTime(scheduleEndHour, scheduleEndMinute));
    ui->limitStockPurchaseCheckBox->setChecked(mConfig->isLimitStockPurchase());
    ui->amountOfStockPurchaseSpinBox->setValue(mConfig->getAmountOfStockPurchase());
    ui->limitByTurnoverCheckBox->setChecked(mConfig->isLimitByTurnover());
    ui->limitByTurnoverPercentDoubleSpinBox->setValue(mConfig->getLimitByTurnoverPercent());
    ui->storageMonthLimitSpinBox->setValue(mConfig->getStorageMonthLimit());
    ui->simulatorConfigCommonCheckBox->setChecked(mConfig->isSimulatorConfigCommon());
    ui->autoPilotConfigCommonCheckBox->setChecked(mConfig->isAutoPilotConfigCommon());
}

void SettingsDialog::on_autorunCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setAutorun(checked);
}

void SettingsDialog::on_makeDecisionTimeoutSpinBox_valueChanged(int value)
{
    mConfig->setMakeDecisionTimeout(value);
}

void SettingsDialog::on_useScheduleCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setUseSchedule(checked);

    ui->scheduleStartTimeEdit->setEnabled(checked);
    ui->scheduleEndTimeEdit->setEnabled(checked);
}

void SettingsDialog::on_scheduleStartTimeEdit_timeChanged(const QTime& time)
{
    if (time > ui->scheduleEndTimeEdit->time())
    {
        ui->scheduleEndTimeEdit->setTime(time);
    }

    mConfig->setScheduleStartHour(time.hour());
    mConfig->setScheduleStartMinute(time.minute());
}

void SettingsDialog::on_scheduleEndTimeEdit_timeChanged(const QTime& time)
{
    if (time < ui->scheduleStartTimeEdit->time())
    {
        ui->scheduleStartTimeEdit->setTime(time);
    }

    mConfig->setScheduleEndHour(time.hour());
    mConfig->setScheduleEndMinute(time.minute());
}

void SettingsDialog::on_limitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setLimitStockPurchase(checked);

    ui->limitStockPurchaseWidget->setEnabled(checked);
}

void SettingsDialog::on_amountOfStockPurchaseSpinBox_valueChanged(int value)
{
    mConfig->setAmountOfStockPurchase(value);
}

void SettingsDialog::on_limitByTurnoverCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setLimitByTurnover(checked);

    ui->limitByTurnoverPercentDoubleSpinBox->setEnabled(checked);
}

void SettingsDialog::on_limitByTurnoverPercentDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLimitByTurnoverPercent(value);
}

void SettingsDialog::on_simulatorConfigCommonCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    if (ui->autoPilotConfigCommonCheckBox->isChecked())
    {
        return; // Unexpected behaviour
    }

    const bool checked = value == Qt::Checked;

    mConfig->setSimulatorConfigCommon(checked);

    if (checked)
    {
        ui->mainTabWidget->setTabText(SIMULATION_TAB_INDEX, tr("Decision maker"));
        ui->mainTabWidget->removeTab(AUTO_PILOT_TAB_INDEX);
    }
    else
    {
        ui->mainTabWidget->insertTab(AUTO_PILOT_TAB_INDEX, ui->autoPilotTab, tr("Auto-pilot"));
        ui->mainTabWidget->setTabText(SIMULATION_TAB_INDEX, tr("Simulation"));
    }
}

void SettingsDialog::on_autoPilotConfigCommonCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    if (ui->simulatorConfigCommonCheckBox->isChecked())
    {
        return; // Unexpected behaviour
    }

    const bool checked = value == Qt::Checked;

    mConfig->setAutoPilotConfigCommon(checked);

    if (checked)
    {
        ui->mainTabWidget->setTabText(AUTO_PILOT_TAB_INDEX, tr("Decision maker"));
        ui->mainTabWidget->removeTab(SIMULATION_TAB_INDEX);
    }
    else
    {
        ui->mainTabWidget->insertTab(SIMULATION_TAB_INDEX, ui->simulationTab, tr("Simulation"));
        ui->mainTabWidget->setTabText(AUTO_PILOT_TAB_INDEX, tr("Auto-pilot"));
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
