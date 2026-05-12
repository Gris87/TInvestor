#include "src/dialogs/settingsdialog/settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>
#include <QSysInfo>



SettingsDialog::SettingsDialog(IConfig* config, QWidget* parent) :
    ISettingsDialog(parent),
    ui(new Ui::SettingsDialog),
    mConfig(config)
{
    qDebug() << "Create SettingsDialog";

    ui->setupUi(this);

    ui->autorunCheckBox->setText(
        tr("Autorun on %1 startup").arg(QSysInfo::productType().at(0).toUpper() + QSysInfo::productType().mid(1))
    );
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "Destroy SettingsDialog";

    delete ui;
}

void SettingsDialog::updateUiFromConfig()
{
    ui->autorunCheckBox->setChecked(mConfig->isAutorun());
    ui->serverAddressLineEdit->setText(mConfig->getServerAddress());
    ui->serverPortSpinBox->setValue(mConfig->getServerPort());
    ui->notificationsEnabledCheckBox->setChecked(mConfig->isNotificationsEnabled());
    ui->filterSystemCheckBox->setChecked(mConfig->isFilterSystem());
    ui->filterPortfolioCheckBox->setChecked(mConfig->isFilterPortfolio());
    ui->filterHugeSellCheckBox->setChecked(mConfig->isFilterHugeSell());
    ui->filterDividendsCheckBox->setChecked(mConfig->isFilterDividends());
    ui->filterPulseNeutralCheckBox->setChecked(mConfig->isFilterPulseNeutral());
    ui->filterPulseBuyCheckBox->setChecked(mConfig->isFilterPulseBuy());
    ui->filterPulseSellCheckBox->setChecked(mConfig->isFilterPulseSell());
}

void SettingsDialog::on_autorunCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setAutorun(checked);
}

void SettingsDialog::on_serverAddressLineEdit_textChanged(const QString& value)
{
    mConfig->setServerAddress(value);
}

void SettingsDialog::on_serverPortSpinBox_valueChanged(int value)
{
    mConfig->setServerPort(value);
}

void SettingsDialog::on_notificationsEnabledCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setNotificationsEnabled(checked);
}

void SettingsDialog::on_filterSystemCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterSystem(checked);
}

void SettingsDialog::on_filterPortfolioCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterPortfolio(checked);
}

void SettingsDialog::on_filterHugeSellCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterHugeSell(checked);
}

void SettingsDialog::on_filterDividendsCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterDividends(checked);
}

void SettingsDialog::on_filterPulseNeutralCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterPulseNeutral(checked);
}

void SettingsDialog::on_filterPulseBuyCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterPulseBuy(checked);
}

void SettingsDialog::on_filterPulseSellCheckBox_checkStateChanged(const Qt::CheckState& value)
{
    const bool checked = value == Qt::Checked;

    mConfig->setFilterPulseSell(checked);
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
