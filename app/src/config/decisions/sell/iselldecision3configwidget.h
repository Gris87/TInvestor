#pragma once



#include <QWidget>

#include "src/config/decisions/sell/iselldecision3config.h"



class ISellDecision3ConfigWidget : public QWidget
{
public:
    ISellDecision3ConfigWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~ISellDecision3ConfigWidget() = default;

    ISellDecision3ConfigWidget(const ISellDecision3ConfigWidget &another) = delete;
    ISellDecision3ConfigWidget& operator=(const ISellDecision3ConfigWidget &another) = delete;

    virtual void setSellDecision3Config(ISellDecision3Config *config) = 0;
    virtual void updateUiFromConfig() = 0;
};
