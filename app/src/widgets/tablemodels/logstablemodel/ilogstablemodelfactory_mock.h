#pragma once



#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory.h"

#include <gmock/gmock.h>



class LogsTableModelFactoryMock : public ILogsTableModelFactory
{
public:
    LogsTableModelFactoryMock() :
        ILogsTableModelFactory()
    {
    }
    ~LogsTableModelFactoryMock() override = default;

    LogsTableModelFactoryMock(const LogsTableModelFactoryMock& another)            = delete;
    LogsTableModelFactoryMock& operator=(const LogsTableModelFactoryMock& another) = delete;

    MOCK_METHOD(ILogsTableModel*, newInstance, (QObject * parent), (const, override));
};
