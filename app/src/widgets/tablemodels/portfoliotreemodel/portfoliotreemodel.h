#pragma once



#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodel.h"

#include <QIcon>



class PortfolioTreeModel : public IPortfolioTreeModel
{
    Q_OBJECT

public:
    explicit PortfolioTreeModel(QObject* parent = nullptr);
    ~PortfolioTreeModel() override;

    PortfolioTreeModel(const PortfolioTreeModel& another)            = delete;
    PortfolioTreeModel& operator=(const PortfolioTreeModel& another) = delete;

    [[nodiscard]]
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QModelIndex parent(const QModelIndex& child) const override;

    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

    void portfolioChanged(const Portfolio& portfolio) override;
    void lastPriceChanged(const QString& instrumentId, float price) override;
    bool updateLastPrices() override;
    void exportToExcel(QXlsx::Document& doc) const override;

    [[nodiscard]]
    double totalCost() const override;

    [[nodiscard]]
    double totalYield() const override;

    [[nodiscard]]
    double totalDailyCost() const override;

    [[nodiscard]]
    double totalDailyYield() const override;

private:
    [[nodiscard]]
    QXlsx::Format createRubleFormat(const QColor& color, bool withPlus, int precision) const;

    [[nodiscard]]
    QXlsx::Format createPercentFormat(const QColor& color, bool withPlus) const;

    void sortCategory(QList<PortfolioItem>* items);
    void reverseCategory(QList<PortfolioItem>* items);
    void updatePriceInCategory(PortfolioCategoryItem* category);

    QStringList          mHeader;
    QIcon                mHelpIcon;
    Portfolio            mPortfolio;
    int                  mSortColumn;
    Qt::SortOrder        mSortOrder;
    QMap<QString, float> mLastPricesUpdates; // Instrument ID => Price
};
