#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision2config.h"



namespace Ui {
class BuyDecision2ConfigWidget;
}



class BuyDecision2ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyDecision2ConfigWidget(QWidget *parent = nullptr);
    ~BuyDecision2ConfigWidget();

    void setBuyDecision2Config(BuyDecision2Config *config);
    void updateUiFromConfig();

private:
    Ui::BuyDecision2ConfigWidget *ui;

    BuyDecision2Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_priceDiffDoubleSpinBox_valueChanged(double value);
    void on_amountOfTimesSpinBox_valueChanged(int value);
    void on_durationSpinBox_valueChanged(int value);
};
