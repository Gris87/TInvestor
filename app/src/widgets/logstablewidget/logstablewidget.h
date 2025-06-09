#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tablerecords/logstablerecord/ilogstablerecordfactory.h"



namespace Ui
{
class LogsTableWidget;
}



class LogsTableWidget : public ILogsTableWidget
{
    Q_OBJECT

public:
    explicit LogsTableWidget(
        ILogsTableRecordFactory*           logsTableRecordFactory,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        IFileDialogFactory*                fileDialogFactory,
        ISettingsEditor*                   settingsEditor,
        QWidget*                           parent = nullptr
    );
    ~LogsTableWidget() override;

    LogsTableWidget(const LogsTableWidget& another)            = delete;
    LogsTableWidget& operator=(const LogsTableWidget& another) = delete;

    Ui::LogsTableWidget* ui;

    void setFilter(LogLevel level) override;
    void logsRead(const QList<LogEntry>& entries) override;
    void logAdded(const LogEntry& entry) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    void exportToExcel(const QString& path) const;

    ILogsTableRecordFactory*           mLogsTableRecordFactory;
    ILogLevelTableItemWidgetFactory*   mLogLevelTableItemWidgetFactory;
    IInstrumentTableItemWidgetFactory* mInstrumentTableItemWidgetFactory;
    IUserStorage*                      mUserStorage;
    IInstrumentsStorage*               mInstrumentsStorage;
    IFileDialogFactory*                mFileDialogFactory;
    ISettingsEditor*                   mSettingsEditor;
    LogLevel                           mFilterLogLevel;
    QList<ILogsTableRecord*>           mRecords;

public slots:
    void on_tableWidget_customContextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
};
