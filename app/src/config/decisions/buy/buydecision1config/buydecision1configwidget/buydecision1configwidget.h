#pragma once



#include "src/config/decisions/buy/buydecision1config/buydecision1configwidget/ibuydecision1configwidget.h"



namespace Ui
{
class BuyDecision1ConfigWidget;
}



class BuyDecision1ConfigWidget : public IBuyDecision1ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision1ConfigWidget(IBuyDecision1Config* config, QWidget* parent = nullptr);
    ~BuyDecision1ConfigWidget();

    BuyDecision1ConfigWidget(const BuyDecision1ConfigWidget& another)            = delete;
    BuyDecision1ConfigWidget& operator=(const BuyDecision1ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;

    Ui::BuyDecision1ConfigWidget* ui;

private:
    IBuyDecision1Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_priceFallDoubleSpinBox_valueChanged(double value);
    void on_durationSpinBox_valueChanged(int value);
};
