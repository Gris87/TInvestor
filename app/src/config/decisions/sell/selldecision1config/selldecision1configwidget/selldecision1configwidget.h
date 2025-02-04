#pragma once



#include "src/config/decisions/sell/selldecision1config/selldecision1configwidget/iselldecision1configwidget.h"



namespace Ui {
class SellDecision1ConfigWidget;
}



class SellDecision1ConfigWidget : public ISellDecision1ConfigWidget
{
    Q_OBJECT

public:
    explicit SellDecision1ConfigWidget(ISellDecision1Config *config, QWidget *parent = nullptr);
    ~SellDecision1ConfigWidget();

    SellDecision1ConfigWidget(const SellDecision1ConfigWidget &another) = delete;
    SellDecision1ConfigWidget& operator=(const SellDecision1ConfigWidget &another) = delete;

    void updateUiFromConfig() override;

    Ui::SellDecision1ConfigWidget *ui;

private:
    ISellDecision1Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
};
