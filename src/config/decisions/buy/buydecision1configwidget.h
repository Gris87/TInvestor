#ifndef BUYDECISION1CONFIGWIDGET_H
#define BUYDECISION1CONFIGWIDGET_H

#include <QWidget>

#include "src/config/buydecision1config.h"



namespace Ui {
class BuyDecision1ConfigWidget;
}



class BuyDecision1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BuyDecision1ConfigWidget(QWidget *parent = nullptr);
    ~BuyDecision1ConfigWidget();

    void setBuyDecision1Config(BuyDecision1Config *config);
    void updateUiFromConfig();

private:
    Ui::BuyDecision1ConfigWidget *ui;

    BuyDecision1Config *mConfig;
};



#endif // BUYDECISION1CONFIGWIDGET_H
