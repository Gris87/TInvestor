#ifndef SELLDECISION2CONFIGWIDGET_H
#define SELLDECISION2CONFIGWIDGET_H

#include <QWidget>

#include "src/config/decisions/sell/selldecision2config.h"



namespace Ui {
class SellDecision2ConfigWidget;
}



class SellDecision2ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SellDecision2ConfigWidget(QWidget *parent = nullptr);
    ~SellDecision2ConfigWidget();

    void setSellDecision2Config(SellDecision2Config *config);
    void updateUiFromConfig();

private:
    Ui::SellDecision2ConfigWidget *ui;

    SellDecision2Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
};



#endif // SELLDECISION2CONFIGWIDGET_H
