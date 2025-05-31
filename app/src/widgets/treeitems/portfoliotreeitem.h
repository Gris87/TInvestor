#pragma once



#include <QTreeWidgetItem>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/instrumentwidget/iinstrumentwidgetfactory.h"



class PortfolioTreeItem : public QTreeWidgetItem
{
public:
    explicit PortfolioTreeItem(
        QTreeWidgetItem*          parent,
        IInstrumentWidgetFactory* instrumentWidgetFactory,
        IUserStorage*             userStorage,
        IInstrumentsStorage*      instrumentsStorage,
        const QString&            instrumentId,
        int                       type = Type
    );
    ~PortfolioTreeItem() override;

    PortfolioTreeItem(const PortfolioTreeItem& another)            = delete;
    PortfolioTreeItem& operator=(const PortfolioTreeItem& another) = delete;

    void setAvailable(double value);
    void setPrice(float value);
    void setAvgPrice(float value);
    void setCost(double value);
    void setPart(float value);
    void setYield(float value);
    void setYieldPercent(float value, float fromPrice);
    void setDailyYield(float value);
    void setCostForDailyYield(double value);
    void setDailyYieldPercent(float value, float fromPrice);

    [[nodiscard]]
    const QString& instrumentId() const;

    [[nodiscard]]
    QString name() const;

    [[nodiscard]]
    double available() const;

    [[nodiscard]]
    float price() const;

    [[nodiscard]]
    float avgPrice() const;

    [[nodiscard]]
    double cost() const;

    [[nodiscard]]
    float part() const;

    [[nodiscard]]
    float yield() const;

    [[nodiscard]]
    float yieldPercent() const;

    [[nodiscard]]
    float dailyYield() const;

    [[nodiscard]]
    float priceForDailyYield() const;

    [[nodiscard]]
    double costForDailyYield() const;

    [[nodiscard]]
    float dailyYieldPercent() const;

    [[nodiscard]]
    qint8 pricePrecision() const;

    bool operator<(const QTreeWidgetItem& another) const override;

private:
    IInstrumentWidget* mInstrumentWidget;
    QString            mInstrumentId;
    double             mAvailable;
    float              mPrice;
    float              mAvgPrice;
    double             mCost;
    float              mPart;
    float              mYield;
    float              mYieldPercent;
    float              mDailyYield;
    double             mCostForDailyYield;
    float              mPriceForDailyYield;
    float              mDailyYieldPercent;
    qint8              mPricePrecision;
};
