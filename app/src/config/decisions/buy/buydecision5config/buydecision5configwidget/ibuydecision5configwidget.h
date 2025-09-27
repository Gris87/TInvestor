#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision5config/ibuydecision5config.h"



class IBuyDecision5ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision5ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision5ConfigWidget() override = default;

    IBuyDecision5ConfigWidget(const IBuyDecision5ConfigWidget& another)            = delete;
    IBuyDecision5ConfigWidget& operator=(const IBuyDecision5ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
