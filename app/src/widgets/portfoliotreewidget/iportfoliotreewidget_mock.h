#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"

#include <gmock/gmock.h>



class PortfolioTreeWidgetMock : public IPortfolioTreeWidget
{
    Q_OBJECT

public:
    explicit PortfolioTreeWidgetMock(QWidget* parent = nullptr) :
        IPortfolioTreeWidget(parent)
    {
    }
    ~PortfolioTreeWidgetMock() override = default;

    PortfolioTreeWidgetMock(const PortfolioTreeWidgetMock& another)            = delete;
    PortfolioTreeWidgetMock& operator=(const PortfolioTreeWidgetMock& another) = delete;

    MOCK_METHOD(void, setAccountName, (const QString& name), (override));

    MOCK_METHOD(void, portfolioChanged, (const Portfolio& portfolio), (override));
    MOCK_METHOD(void, lastPriceChanged, (const QString& instrumentId, float price), (override));
    MOCK_METHOD(void, updateLastPrices, (), (override));

    MOCK_METHOD(void, saveWindowState, (const QString& type), (override));
    MOCK_METHOD(void, loadWindowState, (const QString& type), (override));
};
