#pragma once



#include <QTreeWidgetItem>



class PortfolioTreeItem : public QTreeWidgetItem
{
public:
    explicit PortfolioTreeItem(QTreeWidgetItem* parent, const QString& instrumentId, int type = Type);
    ~PortfolioTreeItem() override;

    PortfolioTreeItem(const PortfolioTreeItem& another)            = delete;
    PortfolioTreeItem& operator=(const PortfolioTreeItem& another) = delete;

    [[nodiscard]]
    const QString& getInstrumentId() const;

    void setAvailable(double value);
    void setPrice(float value);
    void setAvgPrice(float value);
    void setCost(double value);
    void setPart(float value);
    void setYield(float value);
    void setYieldPercent(float value, float fromPrice);
    void setDailyYieldPercent(float value, float fromPrice);

private:
    QString mInstrumentId;
    double  mAvailable;
    float   mPrice;
    float   mAvgPrice;
    double  mCost;
    float   mPart;
    float   mYield;
    float   mYieldPercent;
    float   mDailyYieldPercent;
};
