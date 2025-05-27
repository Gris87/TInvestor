#pragma once



#include <QTreeWidgetItem>



class CategoryTreeItem : public QTreeWidgetItem
{
public:
    explicit CategoryTreeItem(QTreeWidget* treeWidget, const QString& name, int type = Type);
    ~CategoryTreeItem() override;

    CategoryTreeItem(const CategoryTreeItem& another)            = delete;
    CategoryTreeItem& operator=(const CategoryTreeItem& another) = delete;

    void setCost(double value);
    void setPart(float value);

    bool operator<(const QTreeWidgetItem& another) const override;
};
