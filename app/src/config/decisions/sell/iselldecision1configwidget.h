#pragma once



#include <QWidget>

#include "src/config/decisions/sell/iselldecision1config.h"



class ISellDecision1ConfigWidget : public QWidget
{
public:
    ISellDecision1ConfigWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~ISellDecision1ConfigWidget() = default;

    ISellDecision1ConfigWidget(const ISellDecision1ConfigWidget &another) = delete;
    ISellDecision1ConfigWidget& operator=(const ISellDecision1ConfigWidget &another) = delete;

    virtual void updateUiFromConfig() = 0;
};
