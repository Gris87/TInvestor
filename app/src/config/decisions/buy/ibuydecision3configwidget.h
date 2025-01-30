#pragma once



#include <QWidget>

#include "src/config/decisions/buy/ibuydecision3config.h"



class IBuyDecision3ConfigWidget : public QWidget
{
public:
    IBuyDecision3ConfigWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~IBuyDecision3ConfigWidget() = default;

    IBuyDecision3ConfigWidget(const IBuyDecision3ConfigWidget &another) = delete;
    IBuyDecision3ConfigWidget& operator=(const IBuyDecision3ConfigWidget &another) = delete;

    virtual void setBuyDecision3Config(IBuyDecision3Config *config) = 0;
    virtual void updateUiFromConfig() = 0;
};
