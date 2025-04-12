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
    ~IActionsTableItemWidget() override = default;

    IActionsTableItemWidget(const IActionsTableItemWidget& another)            = delete;
    IActionsTableItemWidget& operator=(const IActionsTableItemWidget& another) = delete;
};
