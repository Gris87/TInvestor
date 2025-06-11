#pragma once



#include "src/widgets/logsfilterwidget/ilogsfilterwidget.h"

#include <gmock/gmock.h>



class LogsFilterWidgetMock : public ILogsFilterWidget
{
    Q_OBJECT

public:
    explicit LogsFilterWidgetMock(QWidget* parent = nullptr) :
        ILogsFilterWidget(parent)
    {
    }
    ~LogsFilterWidgetMock() override = default;

    LogsFilterWidgetMock(const LogsFilterWidgetMock& another)            = delete;
    LogsFilterWidgetMock& operator=(const LogsFilterWidgetMock& another) = delete;

    MOCK_METHOD(const LogFilter&, getFilter, (), (override));
};
