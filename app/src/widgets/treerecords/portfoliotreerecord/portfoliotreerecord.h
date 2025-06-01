#pragma once



#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecord.h"

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/treeitems/categorytreeitem.h"
#include "src/widgets/treeitems/portfoliotreeitem.h"



class PortfolioTreeRecord : public IPortfolioTreeRecord
{
    Q_OBJECT

public:
    explicit PortfolioTreeRecord(
        IInstrumentWidgetFactory* instrumentWidgetFactory,
        IUserStorage*             userStorage,
        IInstrumentsStorage*      instrumentsStorage,
        CategoryTreeItem*         categoryTreeItem,
        const QString&            instrumentId,
        QObject*                  parent = nullptr
    );
    ~PortfolioTreeRecord() override;

    PortfolioTreeRecord(const PortfolioTreeRecord& another)            = delete;
    PortfolioTreeRecord& operator=(const PortfolioTreeRecord& another) = delete;

    void setPortfolioItem(const PortfolioItem& item) override;
    void updatePrice(float price) override;
    void exportToExcel(QXlsx::Document& doc, int row) const override;

    [[nodiscard]]
    float yield() const override;

    [[nodiscard]]
    float dailyYield() const override;

private:
    [[nodiscard]]
    QXlsx::Format createRubleFormat(const QColor& color, bool withPlus, int precision) const;

    [[nodiscard]]
    QXlsx::Format createPercentFormat(const QColor& color, bool withPlus) const;

    PortfolioTreeItem* mPortfolioTreeItem;
};
