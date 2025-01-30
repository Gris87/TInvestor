#pragma once



#include "src/config/decisions/sell/iselldecision3configwidget.h"



namespace Ui {
class SellDecision3ConfigWidget;
}



class SellDecision3ConfigWidget : public ISellDecision3ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision3ConfigWidget(QWidget *parent = nullptr);
    ~SellDecision3ConfigWidget();

    SellDecision3ConfigWidget(const SellDecision3ConfigWidget &another) = delete;
    SellDecision3ConfigWidget& operator=(const SellDecision3ConfigWidget &another) = delete;

    void setSellDecision3Config(ISellDecision3Config *config) override;
    void updateUiFromConfig() override;

    Ui::SellDecision3ConfigWidget *ui;

private:
    ISellDecision3Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_durationSpinBox_valueChanged(int value);
    void on_loseIncomeDoubleSpinBox_valueChanged(double value);
};
