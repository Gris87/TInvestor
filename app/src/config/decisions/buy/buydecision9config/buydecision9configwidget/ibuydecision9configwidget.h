#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision9config/ibuydecision9config.h"



class IBuyDecision9ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision9ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision9ConfigWidget() override = default;

    IBuyDecision9ConfigWidget(const IBuyDecision9ConfigWidget& another)            = delete;
    IBuyDecision9ConfigWidget& operator=(const IBuyDecision9ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
