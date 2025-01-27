#pragma once



#include <QWidget>

#include "src/config/decisions/sell/iselldecision1config.h"



namespace Ui {
class SellDecision1ConfigWidget;
}



class SellDecision1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SellDecision1ConfigWidget(QWidget *parent = nullptr);
    ~SellDecision1ConfigWidget();

    SellDecision1ConfigWidget(const SellDecision1ConfigWidget &another) = delete;
    SellDecision1ConfigWidget& operator=(const SellDecision1ConfigWidget &another) = delete;

    void setSellDecision1Config(ISellDecision1Config *config);
    void updateUiFromConfig();

private:
    Ui::SellDecision1ConfigWidget *ui;

    ISellDecision1Config *mConfig;

private slots:
    void on_enabledCheckBox_checkStateChanged(const Qt::CheckState &value);
    void on_incomeAboveDoubleSpinBox_valueChanged(double value);
};
