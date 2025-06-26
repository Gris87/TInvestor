#pragma once



#include "src/widgets/logstablewidget/ilogstablewidget.h"

#include "src/storage/logos/ilogosstorage.h"
#include "src/utils/filedialog/ifiledialogfactory.h"
#include "src/utils/settingseditor/isettingseditor.h"
#include "src/widgets/tablemodels/logstablemodel/ilogstablemodelfactory.h"



namespace Ui
{
class LogsTableWidget;
}



class LogsTableWidget : public ILogsTableWidget
{
    Q_OBJECT

public:
    explicit LogsTableWidget(
        ILogsTableModelFactory* logsTableModelFactory,
        IFileDialogFactory*     fileDialogFactory,
        ISettingsEditor*        settingsEditor,
        QWidget*                parent = nullptr
    );
    ~LogsTableWidget() override;

    LogsTableWidget(const LogsTableWidget& another)            = delete;
    LogsTableWidget& operator=(const LogsTableWidget& another) = delete;

    Ui::LogsTableWidget* ui;

    void setFilter(const LogFilter& filter) override;
    void logsRead(const QList<LogEntry>& entries) override;
    void logAdded(const LogEntry& entry) override;

    void saveWindowState(const QString& type) override;
    void loadWindowState(const QString& type) override;

private:
    void exportToExcel(const QString& path) const;

    IFileDialogFactory* mFileDialogFactory;
    ISettingsEditor*    mSettingsEditor;
    ILogsTableModel*    mLogsTableModel;

public slots:
    void on_tableView_customContextMenuRequested(const QPoint& pos);
    void actionExportToExcelTriggered();
};
