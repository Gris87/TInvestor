#pragma once



#include "src/widgets/tablerecords/logstablerecord/ilogstablerecord.h"

#include <QTableWidget>

#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/widgets/tableitems/instrument/iinstrumenttableitemwidgetfactory.h"
#include "src/widgets/tableitems/loglevel/ilogleveltableitemwidgetfactory.h"
#include "src/widgets/tableitems/timetableitem.h"



class LogsTableRecord : public ILogsTableRecord
{
    Q_OBJECT

public:
    explicit LogsTableRecord(
        QTableWidget*                      tableWidget,
        ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
        IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
        IUserStorage*                      userStorage,
        IInstrumentsStorage*               instrumentsStorage,
        QObject*                           parent = nullptr
    );
    ~LogsTableRecord() override;

    LogsTableRecord(const LogsTableRecord& another)            = delete;
    LogsTableRecord& operator=(const LogsTableRecord& another) = delete;

    void setLogEntry(const LogEntry& entry) override;
    void filter(QTableWidget* tableWidget, const LogFilter& filter) override;
    void exportToExcel(QXlsx::Document& doc) const override;

private:
    IInstrumentsStorage*        mInstrumentsStorage;
    TimeTableItem*              mTimeTableWidgetItem;
    ILogLevelTableItemWidget*   mLevelTableWidgetItem;
    IInstrumentTableItemWidget* mInstrumentTableItemWidget;
    QTableWidgetItem*           mMessageTableWidgetItem;
};
