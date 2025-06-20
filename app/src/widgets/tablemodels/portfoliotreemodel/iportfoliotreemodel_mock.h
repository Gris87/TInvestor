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

    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(int, columnCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(QVariant, data, (const QModelIndex& index, int role), (const, override));
};
