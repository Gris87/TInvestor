#pragma once



#include "src/widgets/tablerecord/itablerecordfactory.h"



class TableRecordFactory : public ITableRecordFactory
{
public:
    TableRecordFactory();
    ~TableRecordFactory();

    TableRecordFactory(const TableRecordFactory& another)            = delete;
    TableRecordFactory& operator=(const TableRecordFactory& another) = delete;

    ITableRecord* newInstance(
        QTableWidget*                   tableWidget,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IOrderWavesDialogFactory*       orderWavesDialogFactory,
        IOrderWavesWidgetFactory*       orderWavesWidgetFactory,
        IOrderBookThread*               orderBookThread,
        IHttpClient*                    httpClient,
        Stock*                          stock,
        QObject*                        parent
    ) override;
};
