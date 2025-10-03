#pragma once



#include "src/config/decisions/sell/selldecision5config/selldecision5configwidget/iselldecision5configwidget.h"



namespace Ui
{

class SellDecision5ConfigWidget;

}



class SellDecision5ConfigWidget : public ISellDecision5ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision5ConfigWidget(ISellDecision5Config* config, QWidget* parent = nullptr);
    ~SellDecision5ConfigWidget() override;

    SellDecision5ConfigWidget(const SellDecision5ConfigWidget& another)            = delete;
    SellDecision5ConfigWidget& operator=(const SellDecision5ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::SellDecision5ConfigWidget* ui;

private:
    ISellDecision5Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_durationSpinBox_valueChanged(int value);
    void on_loseYieldDoubleSpinBox_valueChanged(double value);
};
