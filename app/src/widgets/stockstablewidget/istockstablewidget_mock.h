#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include <gmock/gmock.h>



class StocksTableWidgetMock : public IStocksTableWidget
{
    Q_OBJECT

public:
    explicit StocksTableWidgetMock(QWidget* parent = nullptr) :
        IStocksTableWidget(parent)
    {
    }
    ~StocksTableWidgetMock() override = default;

    StocksTableWidgetMock(const StocksTableWidgetMock& another)            = delete;
    StocksTableWidgetMock& operator=(const StocksTableWidgetMock& another) = delete;

    MOCK_METHOD(void, setFilter, (const StockFilter& filter), (override));
    MOCK_METHOD(void, updateTable, (const QList<Stock*>& stocks, const StockFilter& filter), (override));
    MOCK_METHOD(void, lastPriceChanged, (const QString& instrumentId), (override));
    MOCK_METHOD(void, updateAll, (const StockFilter& filter), (override));
    MOCK_METHOD(void, updateLastPrices, (const StockFilter& filter), (override));
    MOCK_METHOD(void, updatePrices, (const StockFilter& filter), (override));
    MOCK_METHOD(void, updatePeriodicData, (const StockFilter& filter), (override));

    MOCK_METHOD(void, setDateChangeTooltip, (const QString& tooltip), (override));

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
