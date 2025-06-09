#pragma once



#include <QTableWidget>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecord.h"



class ILogsTableRecordFactory
{
public:
    ILogsTableRecordFactory()          = default;
    virtual ~ILogsTableRecordFactory() = default;

    ILogsTableRecordFactory(const ILogsTableRecordFactory& another)            = delete;
    ILogsTableRecordFactory& operator=(const ILogsTableRecordFactory& another) = delete;

    virtual ILogsTableRecord* newInstance(
        QTableWidget*                      tableWidget,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        QObject*                           parent
    ) const = 0;
};
