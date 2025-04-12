#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision3config/ibuydecision3config.h"



class IBuyDecision3ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision3ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision3ConfigWidget() override = default;

    IBuyDecision3ConfigWidget(const IBuyDecision3ConfigWidget& another)            = delete;
    IBuyDecision3ConfigWidget& operator=(const IBuyDecision3ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
};
