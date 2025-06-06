#pragma once



#include <QWidget>

#include "src/config/decisions/sell/selldecision3config/iselldecision3config.h"



class ISellDecision3ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ISellDecision3ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ISellDecision3ConfigWidget() override = default;

    ISellDecision3ConfigWidget(const ISellDecision3ConfigWidget& another)            = delete;
    ISellDecision3ConfigWidget& operator=(const ISellDecision3ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
};
