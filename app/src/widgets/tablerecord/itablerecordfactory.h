#pragma once



#include "src/widgets/tablerecord/itablerecord.h"

#include <QTableWidget>

#include "src/domain/stocks/stock.h"
#include "src/utils/http/ihttpclient.h"
#include "src/widgets/tablerecord/items/actions/iactionstableitemwidgetfactory.h"



class ITableRecordFactory
{
public:
    ITableRecordFactory()
    {
    }
    virtual ~ITableRecordFactory() = default;

    ITableRecordFactory(const ITableRecordFactory& another)            = delete;
    ITableRecordFactory& operator=(const ITableRecordFactory& another) = delete;

    virtual ITableRecord* newInstance(
        QTableWidget*                   tableWidget,
        IActionsTableItemWidgetFactory* actionsTableItemWidgetFactory,
        IHttpClient*                    httpClient,
        Stock*                          stock,
        QObject*                        parent
    ) = 0;
};
