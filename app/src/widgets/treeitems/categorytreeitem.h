#pragma once



#include <QTreeWidgetItem>



class CategoryTreeItem : public QTreeWidgetItem
{
public:
    explicit CategoryTreeItem(int type = Type);
    ~CategoryTreeItem() override;

    CategoryTreeItem(const CategoryTreeItem& another)            = delete;
    CategoryTreeItem& operator=(const CategoryTreeItem& another) = delete;
};
