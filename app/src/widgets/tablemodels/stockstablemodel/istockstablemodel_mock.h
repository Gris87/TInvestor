#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"

#include <gmock/gmock.h>



class StocksTableModelMock : public IStocksTableModel
{
    Q_OBJECT

public:
    explicit StocksTableModelMock(QObject* parent = nullptr) :
        IStocksTableModel(parent)
    {
    }
    ~StocksTableModelMock() override = default;

    StocksTableModelMock(const StocksTableModelMock& another)            = delete;
    StocksTableModelMock& operator=(const StocksTableModelMock& another) = delete;

    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(int, columnCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(QVariant, data, (const QModelIndex& index, int role), (const, override));
};
