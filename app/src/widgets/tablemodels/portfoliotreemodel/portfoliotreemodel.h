#pragma once



#include <QIcon>

#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodel.h"



class PortfolioTreeModel : public IPortfolioTreeModel
{
    Q_OBJECT

public:
    explicit PortfolioTreeModel(QObject* parent = nullptr);
    ~PortfolioTreeModel() override;

    PortfolioTreeModel(const PortfolioTreeModel& another)            = delete;
    PortfolioTreeModel& operator=(const PortfolioTreeModel& another) = delete;

    [[nodiscard]]
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

private:
    QStringList mHeader;
    QIcon       mHelpIcon;
};
