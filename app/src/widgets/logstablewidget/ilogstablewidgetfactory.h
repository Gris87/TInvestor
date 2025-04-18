#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/utils/settingseditor/isettingseditor.h"



class ILogsTableWidgetFactory
{
public:
    ILogsTableWidgetFactory()          = default;
    virtual ~ILogsTableWidgetFactory() = default;

    ILogsTableWidgetFactory(const ILogsTableWidgetFactory& another)            = delete;
    ILogsTableWidgetFactory& operator=(const ILogsTableWidgetFactory& another) = delete;

    virtual ILogsTableWidget* newInstance(ISettingsEditor* settingsEditor, QWidget* parent) const = 0;
};
