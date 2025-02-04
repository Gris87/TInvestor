#pragma once



#include "src/config/decisions/buy/buydecision3config/buydecision3configwidget/ibuydecision3configwidget.h"



namespace Ui {
class BuyDecision3ConfigWidget;
}



class BuyDecision3ConfigWidget : public IBuyDecision3ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision3ConfigWidget(IBuyDecision3Config *config, QWidget *parent = nullptr);
    ~BuyDecision3ConfigWidget();

    BuyDecision3ConfigWidget(const BuyDecision3ConfigWidget &another) = delete;
    BuyDecision3ConfigWidget& operator=(const BuyDecision3ConfigWidget &another) = delete;

    void updateUiFromConfig() override;

    Ui::BuyDecision3ConfigWidget *ui;

private:
    IBuyDecision3Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_priceRiseDoubleSpinBox_valueChanged(double value);
    void on_durationSpinBox_valueChanged(int value);
};
