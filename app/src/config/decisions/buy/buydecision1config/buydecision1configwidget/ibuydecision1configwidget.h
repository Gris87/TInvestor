#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision1config/ibuydecision1config.h"



class IBuyDecision1ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision1ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision1ConfigWidget() override = default;

    IBuyDecision1ConfigWidget(const IBuyDecision1ConfigWidget& another)            = delete;
    IBuyDecision1ConfigWidget& operator=(const IBuyDecision1ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
};
