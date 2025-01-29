#pragma once



#include <QDialog>

#include <QTime>

#include "src/config/config.h"



namespace Ui {
class SettingsDialog;
}



class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(IConfig *config, QWidget *parent = nullptr);
    ~SettingsDialog();

    SettingsDialog(const SettingsDialog &another) = delete;
    SettingsDialog& operator=(const SettingsDialog &another) = delete;

    Ui::SettingsDialog *ui;

private:
    void updateUiFromConfig();

    IConfig *mConfig;

private slots:
    void on_autorunCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_refreshTimeoutSpinBox_valueChanged(int value);
    void on_useScheduleCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_scheduleStartTimeEdit_timeChanged(const QTime &time);
    void on_scheduleEndTimeEdit_timeChanged(const QTime &time);
    void on_limitPurchasesPerDayCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_amountOfPurchasesPerDaySpinBox_valueChanged(int value);
    void on_limitPurchasesPerStockCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_amountOfPurchasesPerStockSpinBox_valueChanged(int value);
    void on_simulatorConfigCommonCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_autoPilotConfigCommonCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_commissionDoubleSpinBox_valueChanged(double value);
    void on_limitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_amountOfStockPurchaseSpinBox_valueChanged(int value);
    void on_storageMonthLimitSpinBox_valueChanged(int value);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_defaultButton_clicked();
};
