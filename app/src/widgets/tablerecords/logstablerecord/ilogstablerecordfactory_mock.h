#pragma once



#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory.h"

#include <gmock/gmock.h>



class LogsTableRecordFactoryMock : public ILogsTableRecordFactory
{
public:
    LogsTableRecordFactoryMock() :
        ILogsTableRecordFactory()
    {
    }
    ~LogsTableRecordFactoryMock() override = default;

    LogsTableRecordFactoryMock(const LogsTableRecordFactoryMock& another)            = delete;
    LogsTableRecordFactoryMock& operator=(const LogsTableRecordFactoryMock& another) = delete;

    MOCK_METHOD(
        ILogsTableRecord*,
        newInstance,
        (QTableWidget * tableWidget,
         ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
         IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
         IUserStorage*                      userStorage,
         IInstrumentsStorage*               instrumentsStorage,
         QObject*                           parent),
        (const, override)
    );
};
