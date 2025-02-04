#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config.h"



class IBuyDecision3ConfigWidget : public QWidget
{
public:
    explicit IBuyDecision3ConfigWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~IBuyDecision3ConfigWidget() = default;

    IBuyDecision3ConfigWidget(const IBuyDecision3ConfigWidget &another) = delete;
    IBuyDecision3ConfigWidget& operator=(const IBuyDecision3ConfigWidget &another) = delete;

    virtual void updateUiFromConfig() = 0;
};
