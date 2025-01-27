#pragma once



#include <QWidget>

#include "src/config/decisions/buy/ibuydecision3config.h"



namespace Ui {
class BuyDecision3ConfigWidget;
}



class BuyDecision3ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyDecision3ConfigWidget(QWidget *parent = nullptr);
    ~BuyDecision3ConfigWidget();

    BuyDecision3ConfigWidget(const BuyDecision3ConfigWidget &another) = delete;
    BuyDecision3ConfigWidget& operator=(const BuyDecision3ConfigWidget &another) = delete;

    void setBuyDecision3Config(IBuyDecision3Config *config);
    void updateUiFromConfig();

private:
    Ui::BuyDecision3ConfigWidget *ui;

    IBuyDecision3Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_priceRiseDoubleSpinBox_valueChanged(double value);
    void on_durationSpinBox_valueChanged(int value);
};
