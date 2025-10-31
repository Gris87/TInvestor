#pragma once



#include "src/widgets/portfoliotreewidget/iportfoliotreewidget.h"

#include <QLabel>

#include "src/domain/trading/tradinginfo.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/messagebox/imessageboxutils.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/portfoliotreemodel/iportfoliotreemodelfactory.h"



namespace Ui
{
class PortfolioTreeWidget;
}



class PortfolioTreeWidget : public IPortfolioTreeWidget
{
    Q_OBJECT

public:
    explicit PortfolioTreeWidget(
        IPortfolioTreeModelFactory* portfolioTreeModelFactory,
        IFileDialogFactory*         fileDialogFactory,
        IMessageBoxUtils*           messageBoxUtils,
        ISettingsEditor*            settingsEditor,
        QWidget*                    parent = nullptr
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
    void updateAllTimeLabel();
    void updateForTodayLabel();
    void updateYieldLabel(QLabel* label, double yield, double cost);
    void sellInstrument(AsapMode mode, const QString& modeText);
    void exportToExcel(const QString& path) const;

    IFileDialogFactory*  mFileDialogFactory;
    IMessageBoxUtils*    mMessageBoxUtils;
    ISettingsEditor*     mSettingsEditor;
    IPortfolioTreeModel* mPortfolioTreeModel;
    double               mTotalCost;
    double               mTotalDailyCost;

public slots:
    void on_treeView_customContextMenuRequested(const QPoint& pos);
    void actionSellAsapTriggered();
    void actionSellFollowPriceTriggered();
    void actionSellGoodYieldTriggered();
    void actionExportToExcelTriggered();
};
