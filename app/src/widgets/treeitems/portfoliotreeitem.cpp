#include "src/widgets/treeitems/portfoliotreeitem.h"

#include <QDebug>

#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



PortfolioTreeItem::PortfolioTreeItem(QTreeWidgetItem* parent, const QString& instrumentId, int type) :
    QTreeWidgetItem(parent, type),
    mInstrumentId(instrumentId)
{
    qDebug() << "Create PortfolioTreeItem";

    setText(PORTFOLIO_NAME_COLUMN, mInstrumentId);
}

PortfolioTreeItem::~PortfolioTreeItem()
{
    qDebug() << "Destroy PortfolioTreeItem";
}

const QString& PortfolioTreeItem::getInstrumentId() const
{
    return mInstrumentId;
}

void PortfolioTreeItem::setAvailable(double value)
{
    setText(PORTFOLIO_AVAILABLE_COLUMN, QString::number(value, 'f', 2));
}

void PortfolioTreeItem::setPrice(float value)
{
    setText(PORTFOLIO_PRICE_COLUMN, QString("%1 \u20BD").arg(value, 0, 'f', 2));
}

void PortfolioTreeItem::setAvgPrice(float value)
{
    setText(PORTFOLIO_AVG_PRICE_COLUMN, QString("%1 \u20BD").arg(value, 0, 'f', 2));
}

void PortfolioTreeItem::setCost(double value)
{
    setText(PORTFOLIO_COST_COLUMN, QString("%1 \u20BD").arg(value, 0, 'f', 2));
}

void PortfolioTreeItem::setPart(float value)
{
    setText(PORTFOLIO_PART_COLUMN, QString("%1 %").arg(value, 0, 'f', 2));
}

void PortfolioTreeItem::setYield(float value)
{
    setText(PORTFOLIO_YIELD_COLUMN, QString("%1 \u20BD").arg(value, 0, 'f', 2));
}

void PortfolioTreeItem::setYieldPercent(float value)
{
    setText(PORTFOLIO_YIELD_PERCENT_COLUMN, QString("%1 %").arg(value, 0, 'f', 2));
}

void PortfolioTreeItem::setDailyYieldPercent(float value)
{
    setText(PORTFOLIO_DAILY_YIELD_COLUMN, QString("%1 %").arg(value, 0, 'f', 2));
}
