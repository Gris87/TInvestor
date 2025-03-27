#pragma once



#include <QWidget>



class IStockTableItemWidget : public QWidget
{
public:
    explicit IStockTableItemWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IStockTableItemWidget() = default;

    IStockTableItemWidget(const IStockTableItemWidget& another)            = delete;
    IStockTableItemWidget& operator=(const IStockTableItemWidget& another) = delete;
};
