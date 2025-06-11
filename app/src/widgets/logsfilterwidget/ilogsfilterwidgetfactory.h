#pragma once



#include "src/widgets/logsfilterwidget/ilogsfilterwidget.h"



class ILogsFilterWidgetFactory
{
public:
    ILogsFilterWidgetFactory()          = default;
    virtual ~ILogsFilterWidgetFactory() = default;

    ILogsFilterWidgetFactory(const ILogsFilterWidgetFactory& another)            = delete;
    ILogsFilterWidgetFactory& operator=(const ILogsFilterWidgetFactory& another) = delete;

    virtual ILogsFilterWidget* newInstance(QWidget* parent) const = 0;
};
