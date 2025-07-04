#pragma once



#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodel.h"

#include <gmock/gmock.h>



class PortfolioTreeModelMock : public IPortfolioTreeModel
{
    Q_OBJECT

public:
    explicit PortfolioTreeModelMock(QObject* parent = nullptr) :
        IPortfolioTreeModel(parent)
    {
    }
    ~PortfolioTreeModelMock() override = default;

    PortfolioTreeModelMock(const PortfolioTreeModelMock& another)            = delete;
    PortfolioTreeModelMock& operator=(const PortfolioTreeModelMock& another) = delete;

    MOCK_METHOD(QModelIndex, index, (int row, int column, const QModelIndex& parent), (const, override));
    MOCK_METHOD(QModelIndex, parent, (const QModelIndex& child), (const, override));
    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(int, columnCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(QVariant, data, (const QModelIndex& index, int role), (const, override));

    MOCK_METHOD(void, portfolioChanged, (const Portfolio& portfolio), (override));
    MOCK_METHOD(void, lastPriceChanged, (const QString& instrumentId, float price), (override));
    MOCK_METHOD(bool, updateLastPrices, (), (override));
    MOCK_METHOD(void, exportToExcel, (QXlsx::Document & doc), (const, override));

    MOCK_METHOD(double, totalCost, (), (const, override));
    MOCK_METHOD(double, totalYield, (), (const, override));
    MOCK_METHOD(double, totalDailyCost, (), (const, override));
    MOCK_METHOD(double, totalDailyYield, (), (const, override));
};
