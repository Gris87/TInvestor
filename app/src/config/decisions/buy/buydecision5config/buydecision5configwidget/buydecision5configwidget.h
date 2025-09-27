#pragma once



#include "src/config/decisions/buy/buydecision5config/buydecision5configwidget/ibuydecision5configwidget.h"



namespace Ui
{

class BuyDecision5ConfigWidget;

}



class BuyDecision5ConfigWidget : public IBuyDecision5ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision5ConfigWidget(IBuyDecision5Config* config, QWidget* parent = nullptr);
    ~BuyDecision5ConfigWidget() override;

    BuyDecision5ConfigWidget(const BuyDecision5ConfigWidget& another)            = delete;
    BuyDecision5ConfigWidget& operator=(const BuyDecision5ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::BuyDecision5ConfigWidget* ui;

private:
    IBuyDecision5Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_priceFallDoubleSpinBox_valueChanged(double value);
    void on_loseYieldDoubleSpinBox_valueChanged(double value);
    void on_durationSpinBox_valueChanged(int value);
};
