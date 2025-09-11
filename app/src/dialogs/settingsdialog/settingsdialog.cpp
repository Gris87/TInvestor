#include "src/dialogs/settingsdialog/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QSysInfo>



constexpr float LIMIT_COMMISSION = 0.06f;

constexpr int SIMULATION_TAB_ID = 1;
constexpr int AUTO_PILOT_TAB_ID = 2;



// clang-format off
static const QMap<int, QString> CPU_USAGE_TO_STRING{ // clazy:exclude=non-pod-global-static
    {0, "MINIMUM"},
    {1, "NORMAL" },
    {2, "OPTIMAL"},
    {3, "MAXIMUM"}
};

static const QMap<QString, int> CPU_USAGE_INDEX{ // clazy:exclude=non-pod-global-static
    {"MINIMUM", 0},
    {"NORMAL",  1},
    {"OPTIMAL", 2},
    {"MAXIMUM", 3}
};
// clang-format on



SettingsDialog::SettingsDialog(
    IConfig*                           config,
    IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
    IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
    IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
    IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
    IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
    ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
    ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
    ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
    ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
    IUserStorage*                      userStorage,
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
        buyDecision4ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        sellDecision4ConfigWidgetFactory,
        ui->simulationTab
    );
    mAutoPilotConfigWidget = decisionMakerConfigWidgetFactory->newInstance(
        mConfig->getAutoPilotConfig(),
        buyDecision1ConfigWidgetFactory,
        buyDecision2ConfigWidgetFactory,
        buyDecision3ConfigWidgetFactory,
        buyDecision4ConfigWidgetFactory,
        sellDecision1ConfigWidgetFactory,
        sellDecision2ConfigWidgetFactory,
        sellDecision3ConfigWidgetFactory,
        sellDecision4ConfigWidgetFactory,
        ui->autoPilotTab
    );

    ui->layoutForSimulatorConfigWidget->addWidget(mSimulatorConfigWidget);
    ui->layoutForAutoPilotConfigWidget->addWidget(mAutoPilotConfigWidget);

    userStorage->readLock();
    const float commission = userStorage->getCommission();
    userStorage->readUnlock();

    ui->autorunCheckBox->setText(
        tr("Autorun on %1 startup").arg(QSysInfo::productType().at(0).toUpper() + QSysInfo::productType().mid(1))
    );

    ui->tradeHugeSpreadCheckBox->setEnabled(commission < LIMIT_COMMISSION);
    ui->hugeSpreadWidget->setEnabled(commission < LIMIT_COMMISSION);
    ui->tradeHugeSpreadCommissionWidget->setVisible(commission >= LIMIT_COMMISSION);
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "Destroy SettingsDialog";

    delete ui;
}

void SettingsDialog::updateUiFromConfig()
{
    mSimulatorConfigWidget->updateUiFromConfig();
    mAutoPilotConfigWidget->updateUiFromConfig();

    ui->autorunCheckBox->setChecked(mConfig->isAutorun());
    ui->cpuUsageComboBox->setCurrentIndex(CPU_USAGE_INDEX.value(mConfig->getCpuUsage(), CPU_USAGE_INDEX.value("MAXIMUM")));
    ui->makeDecisionTimeoutSpinBox->setValue(mConfig->getMakeDecisionTimeout());
    ui->tradeInNonWorkingHoursCheckBox->setChecked(mConfig->isTradeInNonWorkingHours());
    ui->tradeHugeSpreadCheckBox->setChecked(mConfig->isTradeHugeSpread());
    ui->hugeSpreadDoubleSpinBox->setValue(mConfig->getHugeSpread());
    ui->hugeSpreadLimitStockPurchaseCheckBox->setChecked(mConfig->isHugeSpreadLimitStockPurchase());
    ui->hugeSpreadLimitStockPurchasePartDoubleSpinBox->setValue(mConfig->getHugeSpreadLimitStockPurchasePart());
    ui->hugeSpreadLimitByTurnoverCheckBox->setChecked(mConfig->isHugeSpreadLimitByTurnover());
    ui->hugeSpreadLimitByTurnoverPercentDoubleSpinBox->setValue(mConfig->getHugeSpreadLimitByTurnoverPercent());
    ui->tradeLiquidityEtfNightlyCheckBox->setChecked(mConfig->isTradeLiquidityEtfNightly());
    ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->setValue(mConfig->getLiquidityEtfRemainedPartNightly());
    ui->limitStockPurchaseCheckBox->setChecked(mConfig->isLimitStockPurchase());
    ui->limitStockPurchasePartDoubleSpinBox->setValue(mConfig->getLimitStockPurchasePart());
    ui->limitByTurnoverCheckBox->setChecked(mConfig->isLimitByTurnover());
    ui->limitByTurnoverPercentDoubleSpinBox->setValue(mConfig->getLimitByTurnoverPercent());
    ui->limitStockPurchaseNonWorkingHoursCheckBox->setChecked(mConfig->isLimitStockPurchaseNonWorkingHours());
    ui->limitStockPurchasePartNonWorkingHoursDoubleSpinBox->setValue(mConfig->getLimitStockPurchasePartNonWorkingHours());
    ui->limitByTurnoverNonWorkingHoursCheckBox->setChecked(mConfig->isLimitByTurnoverNonWorkingHours());
    ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->setValue(mConfig->getLimitByTurnoverPercentNonWorkingHours());
    ui->storageMonthLimitSpinBox->setValue(mConfig->getStorageMonthLimit());
    ui->simulatorConfigCommonCheckBox->setChecked(mConfig->isSimulatorConfigCommon());
    ui->autoPilotConfigCommonCheckBox->setChecked(mConfig->isAutoPilotConfigCommon());
}

void SettingsDialog::on_autorunCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setAutorun(checked);
}

void SettingsDialog::on_cpuUsageComboBox_currentIndexChanged(int index)
{
    mConfig->setCpuUsage(CPU_USAGE_TO_STRING.value(index));
}

void SettingsDialog::on_makeDecisionTimeoutSpinBox_valueChanged(int value)
{
    mConfig->setMakeDecisionTimeout(value);
}

void SettingsDialog::on_tradeInNonWorkingHoursCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setTradeInNonWorkingHours(checked);

    ui->limitStockPurchaseNonWorkingHoursWidget->setEnabled(checked);
}

void SettingsDialog::on_tradeHugeSpreadCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setTradeHugeSpread(checked);

    ui->hugeSpreadWidget->setEnabled(checked);
}

void SettingsDialog::on_hugeSpreadDoubleSpinBox_valueChanged(double value)
{
    mConfig->setHugeSpread(value);
}

void SettingsDialog::on_hugeSpreadLimitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setHugeSpreadLimitStockPurchase(checked);

    ui->hugeSpreadLimitByTurnoverWidget->setEnabled(checked);
}

void SettingsDialog::on_hugeSpreadLimitStockPurchasePartDoubleSpinBox_valueChanged(double value)
{
    mConfig->setHugeSpreadLimitStockPurchasePart(value);
}

void SettingsDialog::on_hugeSpreadLimitByTurnoverCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setHugeSpreadLimitByTurnover(checked);

    ui->hugeSpreadLimitByTurnoverPercentDoubleSpinBox->setEnabled(checked);
}

void SettingsDialog::on_hugeSpreadLimitByTurnoverPercentDoubleSpinBox_valueChanged(double value)
{
    mConfig->setHugeSpreadLimitByTurnoverPercent(value);
}

void SettingsDialog::on_tradeLiquidityEtfNightlyCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setTradeLiquidityEtfNightly(checked);

    ui->liquidityEtfRemainedPartNightlyDoubleSpinBox->setEnabled(checked);
}

void SettingsDialog::on_liquidityEtfRemainedPartNightlyDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLiquidityEtfRemainedPartNightly(value);
}

void SettingsDialog::on_limitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setLimitStockPurchase(checked);

    ui->limitByTurnoverWidget->setEnabled(checked);
}

void SettingsDialog::on_limitStockPurchasePartDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLimitStockPurchasePart(value);
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

void SettingsDialog::on_limitStockPurchaseNonWorkingHoursCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setLimitStockPurchaseNonWorkingHours(checked);

    ui->limitByTurnoverNonWorkingHoursWidget->setEnabled(checked);
}

void SettingsDialog::on_limitStockPurchasePartNonWorkingHoursDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLimitStockPurchasePartNonWorkingHours(value);
}

void SettingsDialog::on_limitByTurnoverNonWorkingHoursCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setLimitByTurnoverNonWorkingHours(checked);

    ui->limitByTurnoverPercentNonWorkingHoursDoubleSpinBox->setEnabled(checked);
}

void SettingsDialog::on_limitByTurnoverPercentNonWorkingHoursDoubleSpinBox_valueChanged(double value)
{
    mConfig->setLimitByTurnoverPercentNonWorkingHours(value);
}

void SettingsDialog::on_storageMonthLimitSpinBox_valueChanged(int value)
{
    mConfig->setStorageMonthLimit(value);
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
        ui->tabWidget->setTabText(SIMULATION_TAB_ID, tr("Decision maker"));
        ui->tabWidget->removeTab(AUTO_PILOT_TAB_ID);
    }
    else
    {
        ui->tabWidget->insertTab(AUTO_PILOT_TAB_ID, ui->autoPilotTab, tr("Auto-pilot"));
        ui->tabWidget->setTabText(SIMULATION_TAB_ID, tr("Simulation"));
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
        ui->tabWidget->setTabText(AUTO_PILOT_TAB_ID, tr("Decision maker"));
        ui->tabWidget->removeTab(SIMULATION_TAB_ID);
    }
    else
    {
        ui->tabWidget->insertTab(SIMULATION_TAB_ID, ui->simulationTab, tr("Simulation"));
        ui->tabWidget->setTabText(AUTO_PILOT_TAB_ID, tr("Auto-pilot"));
    }
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
