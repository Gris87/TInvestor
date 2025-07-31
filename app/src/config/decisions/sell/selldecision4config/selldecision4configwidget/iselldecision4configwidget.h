#pragma once



#include <QWidget>

#include "src/config/decisions/sell/selldecision4config/iselldecision4config.h"



class ISellDecision4ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ISellDecision4ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~ISellDecision4ConfigWidget() override = default;

    ISellDecision4ConfigWidget(const ISellDecision4ConfigWidget& another)            = delete;
    ISellDecision4ConfigWidget& operator=(const ISellDecision4ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
