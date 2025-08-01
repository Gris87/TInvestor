#pragma once



#include "src/config/decisions/buy/buydecision4config/buydecision4configwidget/ibuydecision4configwidget.h"



namespace Ui
{
class BuyDecision4ConfigWidget;
}



class BuyDecision4ConfigWidget : public IBuyDecision4ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision4ConfigWidget(IBuyDecision4Config* config, QWidget* parent = nullptr);
    ~BuyDecision4ConfigWidget() override;

    BuyDecision4ConfigWidget(const BuyDecision4ConfigWidget& another)            = delete;
    BuyDecision4ConfigWidget& operator=(const BuyDecision4ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::BuyDecision4ConfigWidget* ui;

private:
    IBuyDecision4Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_priceFallDoubleSpinBox_valueChanged(double value);
    void on_loseYieldDoubleSpinBox_valueChanged(double value);
    void on_durationSpinBox_valueChanged(int value);
};
