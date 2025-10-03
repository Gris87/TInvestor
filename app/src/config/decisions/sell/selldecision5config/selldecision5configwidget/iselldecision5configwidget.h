#pragma once



#include <QWidget>

#include "src/config/decisions/sell/selldecision5config/iselldecision5config.h"



class ISellDecision5ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ISellDecision5ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ISellDecision5ConfigWidget() override = default;

    ISellDecision5ConfigWidget(const ISellDecision5ConfigWidget& another)            = delete;
    ISellDecision5ConfigWidget& operator=(const ISellDecision5ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
