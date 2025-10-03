#pragma once



#include "src/dialogs/settingsdialog/isettingsdialog.h"

#include <QTime>

#include "src/config/decisions/decisionmakerconfigwidget/idecisionmakerconfigwidgetfactory.h"
#include "src/config/iconfig.h"
#include "src/storage/user/iuserstorage.h"



namespace Ui
{
class SettingsDialog;
}



class SettingsDialog : public ISettingsDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(
        IConfig*                           config,
        IDecisionMakerConfigWidgetFactory* decisionMakerConfigWidgetFactory,
        IBuyDecision1ConfigWidgetFactory*  buyDecision1ConfigWidgetFactory,
        IBuyDecision2ConfigWidgetFactory*  buyDecision2ConfigWidgetFactory,
        IBuyDecision3ConfigWidgetFactory*  buyDecision3ConfigWidgetFactory,
        IBuyDecision4ConfigWidgetFactory*  buyDecision4ConfigWidgetFactory,
        IBuyDecision5ConfigWidgetFactory*  buyDecision5ConfigWidgetFactory,
        IBuyDecision6ConfigWidgetFactory*  buyDecision6ConfigWidgetFactory,
        IBuyDecision7ConfigWidgetFactory*  buyDecision7ConfigWidgetFactory,
        IBuyDecision8ConfigWidgetFactory*  buyDecision8ConfigWidgetFactory,
        ISellDecision1ConfigWidgetFactory* sellDecision1ConfigWidgetFactory,
        ISellDecision2ConfigWidgetFactory* sellDecision2ConfigWidgetFactory,
        ISellDecision3ConfigWidgetFactory* sellDecision3ConfigWidgetFactory,
        ISellDecision4ConfigWidgetFactory* sellDecision4ConfigWidgetFactory,
        ISellDecision5ConfigWidgetFactory* sellDecision5ConfigWidgetFactory,
        IUserStorage*                      userStorage,
        QWidget*                           parent = nullptr
    );
    ~SettingsDialog() override;

    SettingsDialog(const SettingsDialog& another)            = delete;
    SettingsDialog& operator=(const SettingsDialog& another) = delete;

    void updateUiFromConfig() override;

    Ui::SettingsDialog* ui;

private:
    IConfig*                    mConfig;
    IDecisionMakerConfigWidget* mSimulatorConfigWidget;
    IDecisionMakerConfigWidget* mAutoPilotConfigWidget;

private slots:
    void on_autorunCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_cpuUsageComboBox_currentIndexChanged(int index);
    void on_makeDecisionTimeoutSpinBox_valueChanged(int value);
    void on_tradeInNonWorkingHoursCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_tradeHugeSpreadCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_hugeSpreadDoubleSpinBox_valueChanged(double value);
    void on_hugeSpreadLimitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_hugeSpreadLimitStockPurchasePartDoubleSpinBox_valueChanged(double value);
    void on_hugeSpreadLimitByTurnoverCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_hugeSpreadLimitByTurnoverPercentDoubleSpinBox_valueChanged(double value);
    void on_tradeLiquidityEtfNightlyCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_liquidityEtfRemainedPartNightlyDoubleSpinBox_valueChanged(double value);
    void on_limitStockPurchaseCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_limitStockPurchasePartDoubleSpinBox_valueChanged(double value);
    void on_limitByTurnoverCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_limitByTurnoverPercentDoubleSpinBox_valueChanged(double value);
    void on_limitStockPurchaseNonWorkingHoursCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_limitStockPurchasePartNonWorkingHoursDoubleSpinBox_valueChanged(double value);
    void on_limitByTurnoverNonWorkingHoursCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_limitByTurnoverPercentNonWorkingHoursDoubleSpinBox_valueChanged(double value);
    void on_storageMonthLimitSpinBox_valueChanged(int value);
    void on_highlightGoodOperationsCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_highlightGoodOperationsYieldDoubleSpinBox_valueChanged(double value);
    void on_highlightBadOperationsCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_highlightBadOperationsLoseDoubleSpinBox_valueChanged(double value);
    void on_simulatorConfigCommonCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_autoPilotConfigCommonCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_defaultButton_clicked();
};
