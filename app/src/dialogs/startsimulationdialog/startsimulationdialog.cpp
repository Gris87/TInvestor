#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <QDebug>



#define DATE_FORMAT "yyyy-MM-dd"



StartSimulationDialog::StartSimulationDialog(ISettingsEditor* settingsEditor, QWidget* parent) :
    IStartSimulationDialog(parent),
    ui(new Ui::StartSimulationDialog),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create StartSimulationDialog";

    ui->setupUi(this);

    loadWindowState();
}

StartSimulationDialog::~StartSimulationDialog()
{
    qDebug() << "Destroy StartSimulationDialog";

    delete ui;
}

void StartSimulationDialog::on_dateRangeRadioButton_toggled(bool checked)
{
    ui->dateRangeWidget->setEnabled(checked);
}

void StartSimulationDialog::on_fromDateEdit_dateChanged(const QDate& date)
{
    if (date > ui->toDateEdit->date())
    {
        ui->toDateEdit->setDate(date);
    }
}

void StartSimulationDialog::on_toDateEdit_dateChanged(const QDate& date)
{
    if (date < ui->fromDateEdit->date())
    {
        ui->fromDateEdit->setDate(date);
    }
}

void StartSimulationDialog::on_startButton_clicked()
{
    saveWindowState();

    accept();
}

void StartSimulationDialog::saveWindowState()
{
    qDebug() << "Saving window state";

    // clang-format off
    mSettingsEditor->setValue("StartSimulationDialog/startMoney", ui->startMoneySpinBox->value());
    mSettingsEditor->setValue("StartSimulationDialog/dateRange",  ui->dateRangeRadioButton->isChecked());
    mSettingsEditor->setValue("StartSimulationDialog/fromDate",   ui->fromDateEdit->date().toString(DATE_FORMAT));
    mSettingsEditor->setValue("StartSimulationDialog/toDate",     ui->toDateEdit->date().toString(DATE_FORMAT));
    mSettingsEditor->setValue("StartSimulationDialog/bestConfig", ui->bestConfigCheckBox->isChecked());
    // clang-format on
}

void StartSimulationDialog::loadWindowState()
{
    qDebug() << "Loading window state";

    int currentYear = QDateTime::currentDateTime().date().year();

    QString defaultFromDate = QString("%1-01-01").arg(currentYear - 1);
    QString defaultToDate   = QString("%1-01-01").arg(currentYear);

    // clang-format off
    ui->startMoneySpinBox->setValue(mSettingsEditor->value("StartSimulationDialog/startMoney",           100000).toInt());
    ui->dateRangeRadioButton->setChecked(mSettingsEditor->value("StartSimulationDialog/dateRange",       false).toBool());
    ui->fromDateEdit->setDate(QDate::fromString(mSettingsEditor->value("StartSimulationDialog/fromDate", defaultFromDate).toString(), DATE_FORMAT));
    ui->toDateEdit->setDate(QDate::fromString(mSettingsEditor->value("StartSimulationDialog/toDate",     defaultToDate).toString(),   DATE_FORMAT));
    ui->bestConfigCheckBox->setChecked(mSettingsEditor->value("StartSimulationDialog/bestConfig",        false).toBool());
    // clang-format on
}
