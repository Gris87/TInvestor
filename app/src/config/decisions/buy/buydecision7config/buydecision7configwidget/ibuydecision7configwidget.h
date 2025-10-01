#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision7config/ibuydecision7config.h"



class IBuyDecision7ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision7ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision7ConfigWidget() override = default;

    IBuyDecision7ConfigWidget(const IBuyDecision7ConfigWidget& another)            = delete;
    IBuyDecision7ConfigWidget& operator=(const IBuyDecision7ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
