#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

#include "src/config/config.h"


namespace Ui {
class SettingsDialog;
}



class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(const Config &config, QWidget *parent = nullptr);
    ~SettingsDialog();

    const Config& getConfig();

private:
    Ui::SettingsDialog *ui;

    Config mConfig;
};



#endif // SETTINGSDIALOG_H
