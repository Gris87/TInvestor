#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tablemodels/stockstablemodel/istockstablemodelfactory.h"
#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory.h"



namespace Ui
{
class StocksTableWidget;
}



class StocksTableWidget : public IStocksTableWidget
{
    Q_OBJECT

public:
    explicit StocksTableWidget(
        IStocksTableModelFactory*          stocksTableModelFactory,
        IStocksTableRecordFactory*         stocksTableRecordFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IActionsTableItemWidgetFactory*    actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*          orderWavesDialogFactory,
        IOrderWavesWidgetFactory*          orderWavesWidgetFactory,
        ILogosStorage*                     logosStorage,
        IUserStorage*                      userStorage,
        IOrderBookThread*                  orderBookThread,
        IHttpClient*                       httpClient,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent = nullptr
    );
    ~StocksTableWidget() override;

    StocksTableWidget(const StocksTableWidget& another)            = delete;
    StocksTableWidget& operator=(const StocksTableWidget& another) = delete;

    Ui::StocksTableWidget* ui;

    QMap<QString, IStocksTableRecord*> records;
    QSet<QString>                      lastPricesUpdates;

    void setFilter(const StockFilter& filter) override;
    void updateTable(const QList<Stock*>& stocks, const StockFilter& filter) override;
    void updateAll(const StockFilter& filter) override;
    void lastPriceChanged(const QString& instrumentId) override;
    void updateLastPrices(const StockFilter& filter) override;
    void updatePrices(const StockFilter& filter) override;
    void updatePeriodicData(const StockFilter& filter) override;

    void setDateChangeTooltip(const QString& tooltip) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    void exportToExcel(const QString& path) const;

    IStocksTableRecordFactory*         mStocksTableRecordFactory;
    IInstrumentTableItemWidgetFactory* mInstrumentTableItemWidgetFactory;
    IActionsTableItemWidgetFactory*    mActionsTableItemWidgetFactory;
    IOrderWavesDialogFactory*          mOrderWavesDialogFactory;
    IOrderWavesWidgetFactory*          mOrderWavesWidgetFactory;
    IUserStorage*                      mUserStorage;
    IOrderBookThread*                  mOrderBookThread;
    IHttpClient*                       mHttpClient;
    IFileDialogFactory*                mFileDialogFactory;
    ISettingsEditor*                   mSettingsEditor;
    IStocksTableModel*                 mStocksTableModel;

public slots:
    void on_tableView_customContextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
};
