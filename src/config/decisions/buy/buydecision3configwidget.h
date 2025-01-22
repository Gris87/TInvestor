#ifndef BUYDECISION3CONFIGWIDGET_H
#define BUYDECISION3CONFIGWIDGET_H

#include <QWidget>

#include "src/config/decisions/buy/buydecision3config.h"



namespace Ui {
class BuyDecision3ConfigWidget;
}



class BuyDecision3ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyDecision3ConfigWidget(QWidget *parent = nullptr);
    ~BuyDecision3ConfigWidget();

    void setBuyDecision3Config(BuyDecision3Config *config);
    void updateUiFromConfig();

private:
    Ui::BuyDecision3ConfigWidget *ui;

    BuyDecision3Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
};



#endif // BUYDECISION3CONFIGWIDGET_H
