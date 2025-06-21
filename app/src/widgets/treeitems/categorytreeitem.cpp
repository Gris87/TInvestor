#include "src/widgets/treeitems/categorytreeitem.h"

#include <QDebug>

#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



CategoryTreeItem::CategoryTreeItem(QTreeWidget* treeWidget, const QString& name, int type) :
    QTreeWidgetItem(treeWidget, type),
    mName(name),
    mCost(),
    mPart()
{
    qDebug() << "Create CategoryTreeItem";

    setText(PORTFOLIO_OLD_NAME_COLUMN, mName);
    setExpanded(true);
}

CategoryTreeItem::~CategoryTreeItem()
{
    qDebug() << "Destroy CategoryTreeItem";
}

void CategoryTreeItem::setCost(double value)
{
    mCost = value;

    setText(PORTFOLIO_OLD_COST_COLUMN, QString::number(mCost, 'f', 2) + " \u20BD");
}

void CategoryTreeItem::setPart(float value)
{
    mPart = value;

    setText(PORTFOLIO_OLD_PART_COLUMN, QString::number(mPart, 'f', 2) + "%");
}

QString CategoryTreeItem::name() const
{
    return mName;
}

double CategoryTreeItem::cost() const
{
    return mCost;
}

float CategoryTreeItem::part() const
{
    return mPart;
}

bool CategoryTreeItem::operator<(const QTreeWidgetItem& /*another*/) const
{
    return false;
}
