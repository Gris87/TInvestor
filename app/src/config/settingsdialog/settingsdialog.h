#pragma once



#include "src/config/settingsdialog/isettingsdialog.h"

#include <QTime>

#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
#include "src/config/iconfig.h"



namespace Ui {
class SettingsDialog;
}



class SettingsDialog : public ISettingsDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(
        IConfig *config,
        IDecisionMakerConfigWidgetFactory *decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory *buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory *buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory *buyDecision3ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory *sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory *sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory *sellDecision3ConfigWidgetFactory,
        QWidget *parent = nullptr
    );
    ~SettingsDialog();

    SettingsDialog(const SettingsDialog &another) = delete;
    SettingsDialog& operator=(const SettingsDialog &another) = delete;

    void updateUiFromConfig() override;

    Ui::SettingsDialog *ui;

private:
    IConfig                    *mConfig;
    IDecisionMakerConfigWidget *mSimulatorConfigWidget;
    IDecisionMakerConfigWidget *mAutoPilotConfigWidget;

private slots:
    void on_autorunCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_makeDecisionTimeoutSpinBox_valueChanged(int value);
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
