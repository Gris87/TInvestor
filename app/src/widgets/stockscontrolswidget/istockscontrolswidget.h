#pragma once



#include <QWidget>

#include "src/domain/filter/filter.h"



class IStocksControlsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IStocksControlsWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IStocksControlsWidget() = default;

    IStocksControlsWidget(const IStocksControlsWidget& another)            = delete;
    IStocksControlsWidget& operator=(const IStocksControlsWidget& another) = delete;

    virtual const Filter& getFilter() = 0;

signals:
    void dateChangeDateTimeChanged();
    void filterChanged(const Filter& filter);
};
