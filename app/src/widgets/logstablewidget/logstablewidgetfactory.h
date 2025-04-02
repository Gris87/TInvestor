#pragma once



#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"



class LogsTableWidgetFactory : public ILogsTableWidgetFactory
{
public:
    LogsTableWidgetFactory();
    ~LogsTableWidgetFactory();

    LogsTableWidgetFactory(const LogsTableWidgetFactory& another)            = delete;
    LogsTableWidgetFactory& operator=(const LogsTableWidgetFactory& another) = delete;

    ILogsTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) override;
};
