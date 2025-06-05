#pragma once



#include <QTableWidget>

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
        QTableWidget* tableWidget, ILogLevelTableItemWidgetFactory* logLevelTableItemWidgetFactory, QObject* parent
    ) const = 0;
};
