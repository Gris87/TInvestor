#pragma once



#include <QWidget>



class IActionsTableItemWidget : public QWidget
{
public:
    explicit IActionsTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IActionsTableItemWidget() = default;

    IActionsTableItemWidget(const IActionsTableItemWidget& another)            = delete;
    IActionsTableItemWidget& operator=(const IActionsTableItemWidget& another) = delete;
};
