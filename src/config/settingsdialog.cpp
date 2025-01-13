#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDebug>


SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    qDebug() << "Create SettingsDialog";

    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    qDebug() << "Destroy SettingsDialog";

    delete ui;
}
