#pragma once



#include "src/config/decisions/buy/buydecision6config/buydecision6configwidget/ibuydecision6configwidget.h"



namespace Ui
{

class BuyDecision6ConfigWidget;

}



class BuyDecision6ConfigWidget : public IBuyDecision6ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision6ConfigWidget(IBuyDecision6Config* config, QWidget* parent = nullptr);
    ~BuyDecision6ConfigWidget() override;

    BuyDecision6ConfigWidget(const BuyDecision6ConfigWidget& another)            = delete;
    BuyDecision6ConfigWidget& operator=(const BuyDecision6ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::BuyDecision6ConfigWidget* ui;

private:
    IBuyDecision6Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_durationSpinBox_valueChanged(int value);
};
