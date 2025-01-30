#pragma once



#include <QWidget>

#include "src/config/decisions/buy/ibuydecision2config.h"



class IBuyDecision2ConfigWidget : public QWidget
{
public:
    IBuyDecision2ConfigWidget(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~IBuyDecision2ConfigWidget() = default;

    IBuyDecision2ConfigWidget(const IBuyDecision2ConfigWidget &another) = delete;
    IBuyDecision2ConfigWidget& operator=(const IBuyDecision2ConfigWidget &another) = delete;

    virtual void setBuyDecision2Config(IBuyDecision2Config *config) = 0;
    virtual void updateUiFromConfig() = 0;
};
