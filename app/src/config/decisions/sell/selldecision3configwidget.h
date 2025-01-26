#pragma once



#include <QWidget>

#include "src/config/decisions/sell/selldecision3config.h"



namespace Ui {
class SellDecision3ConfigWidget;
}



class SellDecision3ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SellDecision3ConfigWidget(QWidget *parent = nullptr);
    ~SellDecision3ConfigWidget();

    void setSellDecision3Config(SellDecision3Config *config);
    void updateUiFromConfig();

private:
    Ui::SellDecision3ConfigWidget *ui;

    SellDecision3Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_durationSpinBox_valueChanged(int value);
    void on_loseIncomeDoubleSpinBox_valueChanged(double value);
};
