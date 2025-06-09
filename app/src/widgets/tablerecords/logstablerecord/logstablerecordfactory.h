#pragma once



#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory.h"



class LogsTableRecordFactory : public ILogsTableRecordFactory
{
public:
    LogsTableRecordFactory();
    ~LogsTableRecordFactory() override;

    LogsTableRecordFactory(const LogsTableRecordFactory& another)            = delete;
    LogsTableRecordFactory& operator=(const LogsTableRecordFactory& another) = delete;

    ILogsTableRecord* newInstance(
        QTableWidget*                      tableWidget,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        QObject*                           parent
    ) const override;
};
