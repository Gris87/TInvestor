#pragma once



#include <QWidget>

#include "src/config/decisions/sell/selldecision1config/iselldecision1config.h"



class ISellDecision1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ISellDecision1ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ISellDecision1ConfigWidget() override = default;

    ISellDecision1ConfigWidget(const ISellDecision1ConfigWidget& another)            = delete;
    ISellDecision1ConfigWidget& operator=(const ISellDecision1ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
};
