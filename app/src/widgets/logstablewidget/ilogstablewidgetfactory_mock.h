#pragma once



#include "src/widgets/logstablewidget/ilogstablewidgetfactory.h"

#include <gmock/gmock.h>



class LogsTableWidgetFactoryMock : public ILogsTableWidgetFactory
{
public:
    LogsTableWidgetFactoryMock() :
        ILogsTableWidgetFactory()
    {
    }
    ~LogsTableWidgetFactoryMock() override = default;

    LogsTableWidgetFactoryMock(const LogsTableWidgetFactoryMock& another)            = delete;
    LogsTableWidgetFactoryMock& operator=(const LogsTableWidgetFactoryMock& another) = delete;

    MOCK_METHOD(
        ILogsTableWidget*,
        newInstance,
        (ILogsTableRecordFactory * logsTableRecordFactory,
         IFileDialogFactory* fileDialogFactory,
         ISettingsEditor*    settingsEditor,
         QWidget*            parent),
        (const, override)
    );
};
