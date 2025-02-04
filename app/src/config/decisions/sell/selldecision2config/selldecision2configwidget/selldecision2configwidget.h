#pragma once



#include "src/config/decisions/sell/iselldecision2configwidget.h"



namespace Ui {
class SellDecision2ConfigWidget;
}



class SellDecision2ConfigWidget : public ISellDecision2ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision2ConfigWidget(ISellDecision2Config *config, QWidget *parent = nullptr);
    ~SellDecision2ConfigWidget();

    SellDecision2ConfigWidget(const SellDecision2ConfigWidget &another) = delete;
    SellDecision2ConfigWidget& operator=(const SellDecision2ConfigWidget &another) = delete;

    void updateUiFromConfig() override;

    Ui::SellDecision2ConfigWidget *ui;

private:
    ISellDecision2Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
    void on_loseIncomeDoubleSpinBox_valueChanged(double value);
};
