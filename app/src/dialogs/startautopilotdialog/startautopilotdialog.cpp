#include "src/dialogs/startautopilotdialog/startautopilotdialog.h"
#include "ui_startautopilotdialog.h"

#include <QDebug>



StartAutoPilotDialog::StartAutoPilotDialog(ISettingsEditor* settingsEditor, QWidget* parent) :
    IStartAutoPilotDialog(parent),
    ui(new Ui::StartAutoPilotDialog),
    mSettingsEditor(settingsEditor)
{
    qDebug() << "Create StartAutoPilotDialog";

    ui->setupUi(this);

    loadWindowState();
}

StartAutoPilotDialog::~StartAutoPilotDialog()
{
    qDebug() << "Destroy StartAutoPilotDialog";

    delete ui;
}

void StartAutoPilotDialog::on_accountComboBox_currentIndexChanged(int /*index*/)
{
}

void StartAutoPilotDialog::on_followRadioButton_toggled(bool checked)
{
    ui->followWidget->setEnabled(checked);
}

void StartAutoPilotDialog::on_startButton_clicked()
{
    saveWindowState();

    accept();
}

void StartAutoPilotDialog::saveWindowState()
{
}

void StartAutoPilotDialog::loadWindowState()
{
}
