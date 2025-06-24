#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"

#include <QIcon>

#include "src/domain/stock/stocktableentry.h"



class StocksTableModel : public IStocksTableModel
{
    Q_OBJECT

public:
    explicit StocksTableModel(QObject* parent = nullptr);
    ~StocksTableModel() override;

    StocksTableModel(const StocksTableModel& another)            = delete;
    StocksTableModel& operator=(const StocksTableModel& another) = delete;

    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void setFilter(const StockFilter& filter) override;
    void updateTable(const QList<Stock*>& stocks) override;
    void exportToExcel(QXlsx::Document& doc) const override;

    void setDateChangeTooltip(const QString& tooltip) override;

private:
    [[nodiscard]]
    QXlsx::Format createRubleFormat(const QColor& color, int precision) const;

    [[nodiscard]]
    QXlsx::Format createPercentFormat(const QColor& color, bool withPlus) const;

    void sortEntries();
    void reverseEntries();
    void filterAll();

    QStringList                             mHeader;
    QIcon                                   mHelpIcon;
    QString                                 mDateChangeTooltip;
    StockFilter                             mFilter;
    std::shared_ptr<QList<StockTableEntry>> mEntriesUnfiltered;
    std::shared_ptr<QList<StockTableEntry>> mEntries;
    int                                     mSortColumn;
    Qt::SortOrder                           mSortOrder;
};
