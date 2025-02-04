#pragma once



#include "src/config/decisions/buy/buydecision2config/buydecision2configwidget/ibuydecision2configwidget.h"



namespace Ui {
class BuyDecision2ConfigWidget;
}



class BuyDecision2ConfigWidget : public IBuyDecision2ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision2ConfigWidget(IBuyDecision2Config *config, QWidget *parent = nullptr);
    ~BuyDecision2ConfigWidget();

    BuyDecision2ConfigWidget(const BuyDecision2ConfigWidget &another) = delete;
    BuyDecision2ConfigWidget& operator=(const BuyDecision2ConfigWidget &another) = delete;

    void updateUiFromConfig() override;

    Ui::BuyDecision2ConfigWidget *ui;

private:
    IBuyDecision2Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_priceDiffDoubleSpinBox_valueChanged(double value);
    void on_amountOfTimesSpinBox_valueChanged(int value);
    void on_durationSpinBox_valueChanged(int value);
};
