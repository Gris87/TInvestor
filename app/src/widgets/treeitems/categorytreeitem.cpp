#include "src/widgets/treeitems/categorytreeitem.h"

#include <QDebug>



CategoryTreeItem::CategoryTreeItem(int type) :
    QTreeWidgetItem(type)
{
    qDebug() << "Create CategoryTreeItem";
}

CategoryTreeItem::~CategoryTreeItem()
{
    qDebug() << "Destroy CategoryTreeItem";
}
