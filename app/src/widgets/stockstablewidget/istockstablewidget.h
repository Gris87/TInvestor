#pragma once



#include <QWidget>

#include "src/domain/filter/stockfilter.h"
#include "src/domain/stock/stock.h"



class IStocksTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IStocksTableWidget(QWidget* parent = nullptr) :
        QWidget(parent)
    {
    }
    ~IStocksTableWidget() override = default;

    IStocksTableWidget(const IStocksTableWidget& another)            = delete;
    IStocksTableWidget& operator=(const IStocksTableWidget& another) = delete;

    virtual void updateTable(const QList<Stock*>& stocks, const StockFilter& filter) = 0;

    virtual void updateAll(const StockFilter& filter)          = 0;
    virtual void updateLastPrices(const StockFilter& filter)   = 0;
    virtual void updatePrices(const StockFilter& filter)       = 0;
    virtual void updatePeriodicData(const StockFilter& filter) = 0;

    virtual void setDateChangeTooltip(const QString& tooltip) = 0;

    virtual void lastPriceChanged(const QString& instrumentId) = 0;
    virtual void filterChanged(const StockFilter& filter)      = 0;

    virtual void saveWindowState(const QString& type) = 0;
    virtual void loadWindowState(const QString& type) = 0;
};
