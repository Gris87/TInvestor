#pragma once



#include <QWidget>

#include "src/config/decisions/buy/ibuydecision1config.h"



namespace Ui {
class BuyDecision1ConfigWidget;
}



class BuyDecision1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyDecision1ConfigWidget(QWidget *parent = nullptr);
    ~BuyDecision1ConfigWidget();

    BuyDecision1ConfigWidget(const BuyDecision1ConfigWidget &another) = delete;
    BuyDecision1ConfigWidget& operator=(const BuyDecision1ConfigWidget &another) = delete;

    void setBuyDecision1Config(IBuyDecision1Config *config);
    void updateUiFromConfig();

private:
    Ui::BuyDecision1ConfigWidget *ui;

    IBuyDecision1Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_priceFallDoubleSpinBox_valueChanged(double value);
    void on_durationSpinBox_valueChanged(int value);
};
