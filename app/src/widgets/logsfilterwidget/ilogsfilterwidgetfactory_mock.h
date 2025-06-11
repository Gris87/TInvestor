#pragma once



#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory.h"

#include <gmock/gmock.h>



class LogsFilterWidgetFactoryMock : public ILogsFilterWidgetFactory
{
public:
    LogsFilterWidgetFactoryMock() :
        ILogsFilterWidgetFactory()
    {
    }
    ~LogsFilterWidgetFactoryMock() override = default;

    LogsFilterWidgetFactoryMock(const LogsFilterWidgetFactoryMock& another)            = delete;
    LogsFilterWidgetFactoryMock& operator=(const LogsFilterWidgetFactoryMock& another) = delete;

    MOCK_METHOD(ILogsFilterWidget*, newInstance, (QWidget * parent), (const, override));
};
