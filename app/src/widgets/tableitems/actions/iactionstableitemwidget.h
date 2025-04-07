#pragma once



#include <QWidget>



class IActionsTableItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IActionsTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IActionsTableItemWidget() = default;

    IActionsTableItemWidget(const IActionsTableItemWidget& another)            = delete;
    IActionsTableItemWidget& operator=(const IActionsTableItemWidget& another) = delete;
};
