#include "src/widgets/treeitems/categorytreeitem.h"

#include <QDebug>

#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"



CategoryTreeItem::CategoryTreeItem(QTreeWidget* treeWidget, const QString& name, int type) :
    QTreeWidgetItem(treeWidget, type)
{
    qDebug() << "Create CategoryTreeItem";

    setText(PORTFOLIO_NAME_COLUMN, name);
}

CategoryTreeItem::~CategoryTreeItem()
{
    qDebug() << "Destroy CategoryTreeItem";
}

void CategoryTreeItem::setCost(double value)
{
    setText(PORTFOLIO_COST_COLUMN, QString("%1 \u20BD").arg(value, 0, 'f', 2));
}

void CategoryTreeItem::setPart(float value)
{
    setText(PORTFOLIO_PART_COLUMN, QString("%1 %").arg(value, 0, 'f', 2));
}

bool CategoryTreeItem::operator<(const QTreeWidgetItem& /*another*/) const
{
    return false;
}
