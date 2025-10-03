#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision8config/ibuydecision8config.h"



class IBuyDecision8ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision8ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision8ConfigWidget() override = default;

    IBuyDecision8ConfigWidget(const IBuyDecision8ConfigWidget& another)            = delete;
    IBuyDecision8ConfigWidget& operator=(const IBuyDecision8ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
