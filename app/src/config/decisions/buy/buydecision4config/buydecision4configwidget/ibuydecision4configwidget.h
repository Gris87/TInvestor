#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision4config/ibuydecision4config.h"



class IBuyDecision4ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision4ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision4ConfigWidget() override = default;

    IBuyDecision4ConfigWidget(const IBuyDecision4ConfigWidget& another)            = delete;
    IBuyDecision4ConfigWidget& operator=(const IBuyDecision4ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
    virtual void makeReadOnly() const       = 0;
};
