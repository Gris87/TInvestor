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
