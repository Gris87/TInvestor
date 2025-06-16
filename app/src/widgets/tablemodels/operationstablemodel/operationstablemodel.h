#pragma once



#include "src/widgets/tablemodels/operationstablemodel/ioperationstablemodel.h"



class OperationsTableModel : public IOperationsTableModel
{
    Q_OBJECT

public:
    explicit OperationsTableModel(QObject* parent = nullptr);
    ~OperationsTableModel() override;

    OperationsTableModel(const OperationsTableModel& another)            = delete;
    OperationsTableModel& operator=(const OperationsTableModel& another) = delete;

    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void operationsRead(const QList<Operation>& operations) override;
    void operationsAdded(const QList<Operation>& operations) override;
    void exportToExcel(QXlsx::Document& doc) const override;

private:
    int  indexOfSortedInsert(QList<Operation>* entries, const Operation& entry);
    void insertRow(QList<Operation>* entries, int row, const Operation& entry);

    QStringList                       mHeader;
    std::shared_ptr<QList<Operation>> mEntries;
    int                               mSortColumn;
    Qt::SortOrder                     mSortOrder;
};
