#pragma once



#include <QWidget>

#include "src/config/decisions/sell/selldecision2config/iselldecision2config.h"



class ISellDecision2ConfigWidget : public QWidget
{
public:
    explicit ISellDecision2ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~ISellDecision2ConfigWidget() = default;

    ISellDecision2ConfigWidget(const ISellDecision2ConfigWidget& another)            = delete;
    ISellDecision2ConfigWidget& operator=(const ISellDecision2ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() = 0;
};
