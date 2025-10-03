#pragma once



#include "src/config/decisions/buy/buydecision8config/buydecision8configwidget/ibuydecision8configwidget.h"



namespace Ui
{

class BuyDecision8ConfigWidget;

}



class BuyDecision8ConfigWidget : public IBuyDecision8ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision8ConfigWidget(IBuyDecision8Config* config, QWidget* parent = nullptr);
    ~BuyDecision8ConfigWidget() override;

    BuyDecision8ConfigWidget(const BuyDecision8ConfigWidget& another)            = delete;
    BuyDecision8ConfigWidget& operator=(const BuyDecision8ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::BuyDecision8ConfigWidget* ui;

private:
    IBuyDecision8Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_durationSpinBox_valueChanged(int value);
};
