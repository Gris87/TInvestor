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
