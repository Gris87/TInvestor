#pragma once



#include <QWidget>

#include "src/config/decisions/sell/iselldecision2config.h"



namespace Ui {
class SellDecision2ConfigWidget;
}



class SellDecision2ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SellDecision2ConfigWidget(QWidget *parent = nullptr);
    ~SellDecision2ConfigWidget();

    SellDecision2ConfigWidget(const SellDecision2ConfigWidget &another) = delete;
    SellDecision2ConfigWidget& operator=(const SellDecision2ConfigWidget &another) = delete;

    void setSellDecision2Config(ISellDecision2Config *config);
    void updateUiFromConfig();

    Ui::SellDecision2ConfigWidget *ui;

private:
    ISellDecision2Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
    void on_loseIncomeDoubleSpinBox_valueChanged(double value);
};
