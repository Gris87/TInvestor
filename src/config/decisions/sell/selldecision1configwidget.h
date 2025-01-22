#ifndef SELLDECISION1CONFIGWIDGET_H
#define SELLDECISION1CONFIGWIDGET_H

#include <QWidget>

#include "src/config/decisions/sell/selldecision1config.h"



namespace Ui {
class SellDecision1ConfigWidget;
}



class SellDecision1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SellDecision1ConfigWidget(QWidget *parent = nullptr);
    ~SellDecision1ConfigWidget();

    void setSellDecision1Config(SellDecision1Config *config);
    void updateUiFromConfig();

private:
    Ui::SellDecision1ConfigWidget *ui;

    SellDecision1Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
};



#endif // SELLDECISION1CONFIGWIDGET_H
