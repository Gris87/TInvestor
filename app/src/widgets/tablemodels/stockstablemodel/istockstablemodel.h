#pragma once



#include <QAbstractTableModel>



enum StocksTableColumns : qint8
{
    STOCKS_STOCK_COLUMN,
    STOCKS_PRICE_COLUMN,
    STOCKS_DAY_CHANGE_COLUMN,
    STOCKS_DATE_CHANGE_COLUMN,
    STOCKS_TURNOVER_COLUMN,
    STOCKS_PAYBACK_COLUMN,
    STOCKS_ACTIONS_COLUMN,
    STOCKS_COLUMN_COUNT
};



class IStocksTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit IStocksTableModel(QObject* parent = nullptr) :
        QAbstractTableModel(parent)
    {
    }
    ~IStocksTableModel() override = default;

    IStocksTableModel(const IStocksTableModel& another)            = delete;
    IStocksTableModel& operator=(const IStocksTableModel& another) = delete;

    virtual void setDateChangeTooltip(const QString& tooltip) = 0;
};
