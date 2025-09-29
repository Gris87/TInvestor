#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision6config/ibuydecision6config.h"



class IBuyDecision6ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision6ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision6ConfigWidget() override = default;

    IBuyDecision6ConfigWidget(const IBuyDecision6ConfigWidget& another)            = delete;
    IBuyDecision6ConfigWidget& operator=(const IBuyDecision6ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
