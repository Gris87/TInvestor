#pragma once



#include "src/widgets/tablemodels/stockstablemodel/istockstablemodel.h"

#include <QIcon>



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

    void setDateChangeTooltip(const QString& tooltip) override;

private:
    QStringList mHeader;
    QIcon       mHelpIcon;
    QString     mDateChangeTooltip;
};
