#pragma once



#include "src/config/decisions/buy/buydecision9config/buydecision9configwidget/ibuydecision9configwidget.h"



namespace Ui
{

class BuyDecision9ConfigWidget;

}



class BuyDecision9ConfigWidget : public IBuyDecision9ConfigWidget
{
    Q_OBJECT

public:
    explicit BuyDecision9ConfigWidget(IBuyDecision9Config* config, QWidget* parent = nullptr);
    ~BuyDecision9ConfigWidget() override;

    BuyDecision9ConfigWidget(const BuyDecision9ConfigWidget& another)            = delete;
    BuyDecision9ConfigWidget& operator=(const BuyDecision9ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::BuyDecision9ConfigWidget* ui;

private:
    IBuyDecision9Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_rsiDoubleSpinBox_valueChanged(double value);
};
