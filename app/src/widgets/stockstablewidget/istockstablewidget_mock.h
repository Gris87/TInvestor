#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include <gmock/gmock.h>



class StocksTableWidgetMock : public IStocksTableWidget
{
public:
    explicit StocksTableWidgetMock(QWidget* parent = nullptr) :
        IStocksTableWidget(parent)
    {
    }
    ~StocksTableWidgetMock() override = default;

    StocksTableWidgetMock(const StocksTableWidgetMock& another)            = delete;
    StocksTableWidgetMock& operator=(const StocksTableWidgetMock& another) = delete;

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
