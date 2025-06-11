#pragma once



#include "src/widgets/stockscontrolswidget/istockscontrolswidget.h"

#include <gmock/gmock.h>



class StocksControlsWidgetMock : public IStocksControlsWidget
{
    Q_OBJECT

public:
    explicit StocksControlsWidgetMock(QWidget* parent = nullptr) :
        IStocksControlsWidget(parent)
    {
    }
    ~StocksControlsWidgetMock() override = default;

    StocksControlsWidgetMock(const StocksControlsWidgetMock& another)            = delete;
    StocksControlsWidgetMock& operator=(const StocksControlsWidgetMock& another) = delete;

    MOCK_METHOD(QDateTime, getDateChangeTime, (), (override));
    MOCK_METHOD(const StockFilter&, getFilter, (), (override));

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
