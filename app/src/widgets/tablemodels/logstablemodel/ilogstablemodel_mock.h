#pragma once



#include "src/widgets/tablemodels/logstablemodel/ilogstablemodel.h"

#include <gmock/gmock.h>



class LogsTableModelMock : public ILogsTableModel
{
    Q_OBJECT

public:
    explicit LogsTableModelMock(QObject* parent = nullptr) :
        ILogsTableModel(parent)
    {
    }
    ~LogsTableModelMock() override = default;

    LogsTableModelMock(const LogsTableModelMock& another)            = delete;
    LogsTableModelMock& operator=(const LogsTableModelMock& another) = delete;
};
