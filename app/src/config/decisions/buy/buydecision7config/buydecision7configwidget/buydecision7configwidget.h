#pragma once



#include "src/config/decisions/buy/buydecision7config/buydecision7configwidget/ibuydecision7configwidget.h"



namespace Ui
{

class BuyDecision7ConfigWidget;

}



class BuyDecision7ConfigWidget : public IBuyDecision7ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision7ConfigWidget(IBuyDecision7Config* config, QWidget* parent = nullptr);
    ~BuyDecision7ConfigWidget() override;

    BuyDecision7ConfigWidget(const BuyDecision7ConfigWidget& another)            = delete;
    BuyDecision7ConfigWidget& operator=(const BuyDecision7ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::BuyDecision7ConfigWidget* ui;

private:
    IBuyDecision7Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_priceRaiseDoubleSpinBox_valueChanged(double value);
    void on_orderBookPositionsSpinBox_valueChanged(int value);
    void on_durationSpinBox_valueChanged(int value);
};
