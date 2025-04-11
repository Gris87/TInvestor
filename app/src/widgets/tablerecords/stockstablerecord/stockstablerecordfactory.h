#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory.h"



class StocksTableRecordFactory : public IStocksTableRecordFactory
{
public:
    StocksTableRecordFactory();
    ~StocksTableRecordFactory();

    StocksTableRecordFactory(const StocksTableRecordFactory& another)            = delete;
    StocksTableRecordFactory& operator=(const StocksTableRecordFactory& another) = delete;

    IStocksTableRecord* newInstance(
        QTableWidget*                   tableWidget,
        IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IUserStorage*                   userStorage,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        Stock*                          stock,
        QObject*                        parent
    ) const override;
};
