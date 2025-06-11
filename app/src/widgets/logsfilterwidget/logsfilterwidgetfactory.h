#pragma once



#include "src/widgets/logsfilterwidget/ilogsfilterwidgetfactory.h"



class LogsFilterWidgetFactory : public ILogsFilterWidgetFactory
{
public:
    LogsFilterWidgetFactory();
    ~LogsFilterWidgetFactory() override;

    LogsFilterWidgetFactory(const LogsFilterWidgetFactory& another)            = delete;
    LogsFilterWidgetFactory& operator=(const LogsFilterWidgetFactory& another) = delete;

    ILogsFilterWidget* newInstance(QWidget* parent) const override;
};
