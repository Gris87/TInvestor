#include "src/widgets/tablerecords/logstablerecord/logstablerecordfactory.h"

#include <QDebug>

#include "src/widgets/tablerecords/logstablerecord/logstablerecord.h"



LogsTableRecordFactory::LogsTableRecordFactory() :
    ILogsTableRecordFactory()
{
    qDebug() << "Create LogsTableRecordFactory";
}

LogsTableRecordFactory::~LogsTableRecordFactory()
{
    qDebug() << "Destroy LogsTableRecordFactory";
}

ILogsTableRecord* LogsTableRecordFactory::newInstance(
    QTableWidget*                      tableWidget,
    ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    QObject*                           parent
) const
{
    return new LogsTableRecord(
        tableWidget, logLevelTableItemWidgetFactory, instrumentTableItemWidgetFactory, userStorage, instrumentsStorage, parent
    );
}
