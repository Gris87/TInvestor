#pragma once



#include "src/widgets/stockstablewidget/istockstablewidget.h"

#include "src/dialogs/orderwavesdialog/iorderwavesdialogfactory.h"
#include "src/storage/user/iuserstorage.h"
#include "src/threads/orderbook/iorderbookthread.h"
#include "src/utils/http/ihttpclient.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/orderwaveswidget/iorderwaveswidgetfactory.h"
#include "src/widgets/tableitems/actions/iactionstableitemwidgetfactory.h"
#include "src/widgets/tableitems/stock/istocktableitemwidgetfactory.h"
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
        IStocksTableRecordFactory*      stockTableRecordFactory,
        IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IUserStorage*                   userStorage,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        ISettingsEditor*                settingsEditor,
        QWidget*                        parent = nullptr
    );
    ~StocksTableWidget() override;

    StocksTableWidget(const StocksTableWidget& another)            = delete;
    StocksTableWidget& operator=(const StocksTableWidget& another) = delete;

    Ui::StocksTableWidget* ui;

    QMap<QString, IStocksTableRecord*> tableRecords;
    QSet<QString>                      lastPricesUpdates;

    void updateTable(const QList<Stock*>& stocks, const Filter& filter) override;

    void updateAll(const Filter& filter) override;
    void updateLastPrices(const Filter& filter) override;
    void updatePrices(const Filter& filter) override;
    void updatePeriodicData(const Filter& filter) override;

    void setDateChangeTooltip(const QString& tooltip) override;

    void lastPriceChanged(const QString& instrumentId) override;
    void filterChanged(const Filter& filter) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    IStocksTableRecordFactory*      mStocksTableRecordFactory;
    IStockTableItemWidgetFactory*   mStockTableItemWidgetFactory;
    IActionsTableItemWidgetFactory* mActionsTableItemWidgetFactory;
    IOrderWavesDialogFactory*       mOrderWavesDialogFactory;
    IOrderWavesWidgetFactory*       mOrderWavesWidgetFactory;
    IUserStorage*                   mUserStorage;
    IOrderBookThread*               mOrderBookThread;
    IHttpClient*                    mHttpClient;
    ISettingsEditor*                mSettingsEditor;
};
