#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"

#include <QLabel>

#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/treeitems/categorytreeitem.h"
#include "src/widgets/treerecords/portfoliotreerecord/iportfoliotreerecordfactory.h"



namespace Ui
{
class PortfolioTreeWidget;
}



class PortfolioTreeWidget : public IPortfolioTreeWidget
{
    Q_OBJECT

public:
    explicit PortfolioTreeWidget(
        IPortfolioTreeRecordFactory* portfolioTreeRecordFactory,
        IInstrumentWidgetFactory*    instrumentWidgetFactory,
        IUserStorage*                userStorage,
        IInstrumentsStorage*         instrumentsStorage,
        IFileDialogFactory*          fileDialogFactory,
        ISettingsEditor*             settingsEditor,
        QWidget*                     parent = nullptr
    );
    ~PortfolioTreeWidget() override;

    PortfolioTreeWidget(const PortfolioTreeWidget& another)            = delete;
    PortfolioTreeWidget& operator=(const PortfolioTreeWidget& another) = delete;

    Ui::PortfolioTreeWidget* ui;

    void setAccountName(const QString& name) override;

    void portfolioChanged(const Portfolio& portfolio) override;
    void lastPriceChanged(const QString& instrumentId, float price) override;
    void updateLastPrices() override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    void deleteObsoleteCategories(const Portfolio& portfolio);
    void updateCategory(CategoryTreeItem* categoryTreeItem, const PortfolioItems& portfolioItems);
    void deleteObsoleteRecords(CategoryTreeItem* categoryTreeItem, const PortfolioItems& portfolioItems);
    void updateAllTimeLabel();
    void updateForTodayLabel();
    void updateYieldLabel(QLabel* label, double yield, double cost);
    void exportToExcel(const QString& path) const;

    IPortfolioTreeRecordFactory*         mPortfolioTreeRecordFactory;
    IInstrumentWidgetFactory*            mInstrumentWidgetFactory;
    IUserStorage*                        mUserStorage;
    IInstrumentsStorage*                 mInstrumentsStorage;
    IFileDialogFactory*                  mFileDialogFactory;
    ISettingsEditor*                     mSettingsEditor;
    QStringList                          mSortedCategories;
    QMap<QString, QString>               mCategoryNames;     // Category => Localized name
    QMap<QString, CategoryTreeItem*>     mCategories;        // Category => CategoryTreeItem
    QMap<QString, IPortfolioTreeRecord*> mRecords;           // UID => PortfolioTreeRecord
    QMap<QString, float>                 mLastPricesUpdates; // UID => Price
    double                               mTotalCost;
    double                               mTotalYield;
    double                               mTotalDailyCost;
    double                               mTotalDailyYield;

public slots:
    void on_treeWidget_customContextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
};
