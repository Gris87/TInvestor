#pragma once



#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodel.h"

#include <gmock/gmock.h>



class OperationsTableModelMock : public IOperationsTableModel
{
    Q_OBJECT

public:
    explicit OperationsTableModelMock(QObject* parent = nullptr) :
        IOperationsTableModel(parent)
    {
    }
    ~OperationsTableModelMock() override = default;

    OperationsTableModelMock(const OperationsTableModelMock& another)            = delete;
    OperationsTableModelMock& operator=(const OperationsTableModelMock& another) = delete;

    MOCK_METHOD(int, rowCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(int, columnCount, (const QModelIndex& parent), (const, override));
    MOCK_METHOD(QVariant, data, (const QModelIndex& index, int role), (const, override));

    MOCK_METHOD(void, operationsRead, (const QList<Operation>& operations), (override));
    MOCK_METHOD(void, operationsAdded, (const QList<Operation>& operations), (override));
    MOCK_METHOD(void, exportToExcel, (QXlsx::Document & doc), (const, override));
};
