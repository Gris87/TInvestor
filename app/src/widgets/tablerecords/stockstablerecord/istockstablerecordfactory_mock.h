#pragma once



#include "src/widgets/tablerecords/stockstablerecord/istockstablerecordfactory.h"

#include <gmock/gmock.h>



class StocksTableRecordFactoryMock : public IStocksTableRecordFactory
{
public:
    StocksTableRecordFactoryMock() :
        IStocksTableRecordFactory()
    {
    }
    ~StocksTableRecordFactoryMock() override = default;

    StocksTableRecordFactoryMock(const StocksTableRecordFactoryMock& another)            = delete;
    StocksTableRecordFactoryMock& operator=(const StocksTableRecordFactoryMock& another) = delete;

    MOCK_METHOD(
        IStocksTableRecord*,
        newInstance,
        (QTableWidget * tableWidget,
         IStockTableItemWidgetFactory*   stockTableItemWidgetFactory,
         IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
         IOrderWavesDialogFactory*       orderWavesDialogFactory,
         IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
         IUserStorage*                   userStorage,
         IOrderBookThread*               orderBookThread,
         IHttpClient*                    httpClient,
         Stock*                          stock,
         QObject*                        parent),
        (override)
    );
};
