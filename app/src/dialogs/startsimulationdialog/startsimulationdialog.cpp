#include "src/dialogs/startsimulationdialog/startsimulationdialog.h"
#include "ui_startsimulationdialog.h"

#include <QDebug>



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
}

void StartSimulationDialog::loadWindowState()
{
}
