#pragma once



#include "src/widgets/tablerecord/items/stock/istocktableitemwidget.h"

#include <gmock/gmock.h>



class StockTableItemWidgetMock : public IStockTableItemWidget
{
public:
    explicit StockTableItemWidgetMock(QWidget* parent = nullptr) :
        IStockTableItemWidget(parent)
    {
    }
    ~StockTableItemWidgetMock() override = default;

    StockTableItemWidgetMock(const StockTableItemWidgetMock& another)            = delete;
    StockTableItemWidgetMock& operator=(const StockTableItemWidgetMock& another) = delete;

    MOCK_METHOD(void, setIcon, (const QIcon& icon), (override));
    MOCK_METHOD(void, setQualInvestor, (bool forQualInvestorFlag), (override));
    MOCK_METHOD(void, setText, (const QString& text), (override));
    MOCK_METHOD(void, setFullText, (const QString& text), (override));

    MOCK_METHOD(bool, forQualInvestorFlag, (), (override));
    MOCK_METHOD(QString, text, (), (override));
    MOCK_METHOD(QString, fullText, (), (override));
};
