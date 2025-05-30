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

    [[nodiscard]]
    QString name() const;

    [[nodiscard]]
    double cost() const;

    [[nodiscard]]
    float part() const;

    bool operator<(const QTreeWidgetItem& another) const override;

private:
    QString mName;
    double  mCost;
    float   mPart;
};
