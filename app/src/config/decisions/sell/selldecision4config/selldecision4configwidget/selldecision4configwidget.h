#pragma once



#include "src/config/decisions/sell/selldecision4config/selldecision4configwidget/iselldecision4configwidget.h"



namespace Ui
{
class SellDecision4ConfigWidget;
}



class SellDecision4ConfigWidget : public ISellDecision4ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision4ConfigWidget(ISellDecision4Config* config, QWidget* parent = nullptr);
    ~SellDecision4ConfigWidget() override;

    SellDecision4ConfigWidget(const SellDecision4ConfigWidget& another)            = delete;
    SellDecision4ConfigWidget& operator=(const SellDecision4ConfigWidget& another) = delete;

    void updateUiFromConfig() const override;
    void makeReadOnly() const override;

    Ui::SellDecision4ConfigWidget* ui;

private:
    ISellDecision4Config* mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState& value);
    void on_durationSpinBox_valueChanged(int value);
    void on_yieldAboveDoubleSpinBox_valueChanged(double value);
};
