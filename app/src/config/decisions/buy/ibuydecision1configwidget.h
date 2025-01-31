#pragma once



#include <QWidget>

#include "src/config/decisions/buy/ibuydecision1config.h"



class IBuyDecision1ConfigWidget : public QWidget
{
public:
    IBuyDecision1ConfigWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~IBuyDecision1ConfigWidget() = default;

    IBuyDecision1ConfigWidget(const IBuyDecision1ConfigWidget &another) = delete;
    IBuyDecision1ConfigWidget& operator=(const IBuyDecision1ConfigWidget &another) = delete;

    virtual void updateUiFromConfig() = 0;
};
