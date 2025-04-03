#pragma once



#include <QWidget>

#include "src/domain/filter/filter.h"
#include "src/domain/stocks/stock.h"



class IStocksTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IStocksTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    virtual ~IStocksTableWidget() = default;

    IStocksTableWidget(const IStocksTableWidget& another)            = delete;
    IStocksTableWidget& operator=(const IStocksTableWidget& another) = delete;

    virtual void updateTable(const QList<Stock*>& stocks, const Filter& filter) = 0;

    virtual void updateAll(const Filter& filter)          = 0;
    virtual void updateLastPrices(const Filter& filter)   = 0;
    virtual void updatePrices(const Filter& filter)       = 0;
    virtual void updatePeriodicData(const Filter& filter) = 0;

    virtual void setDateChangeTooltip(const QString& tooltip) = 0;

    virtual void lastPriceChanged(const QString& uid) = 0;
    virtual void filterChanged(const Filter& filter)  = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
