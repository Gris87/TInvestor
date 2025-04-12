#pragma once



#include <QWidget>

#include "src/config/decisions/buy/buydecision2config/ibuydecision2config.h"



class IBuyDecision2ConfigWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IBuyDecision2ConfigWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IBuyDecision2ConfigWidget() override = default;

    IBuyDecision2ConfigWidget(const IBuyDecision2ConfigWidget& another)            = delete;
    IBuyDecision2ConfigWidget& operator=(const IBuyDecision2ConfigWidget& another) = delete;

    virtual void updateUiFromConfig() const = 0;
};
