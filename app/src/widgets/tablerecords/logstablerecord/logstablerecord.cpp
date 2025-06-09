#include "src/widgets/tablerecords/logstablerecord/logstablerecord.h"

#include <QCoreApplication>
#include <QDebug>



const char* const DATETIME_FORMAT       = "yyyy-MM-dd hh:mm:ss";
const QColor      CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



LogsTableRecord::LogsTableRecord(
    QTableWidget*                      tableWidget,
    ILogLevelTableItemWidgetFactory*   logLevelTableItemWidgetFactory,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    QObject*                           parent
) :
    ILogsTableRecord(parent),
    mInstrumentsStorage(instrumentsStorage),
    mTimeTableWidgetItem(new TimeTableItem()),
    mLevelTableWidgetItem(),
    mInstrumentTableItemWidget(),
    mMessageTableWidgetItem(new QTableWidgetItem())
{
    qDebug() << "Create LogsTableRecord";

    mLevelTableWidgetItem = logLevelTableItemWidgetFactory->newInstance(tableWidget); // tableWidget will take ownership
    mInstrumentTableItemWidget =
        instrumentTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    // clang-format off
    tableWidget->setItem(rowIndex,       LOGS_TIME_COLUMN,    mTimeTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, LOGS_LEVEL_COLUMN,   mLevelTableWidgetItem);
    tableWidget->setItem(rowIndex,       LOGS_LEVEL_COLUMN,   mLevelTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, LOGS_NAME_COLUMN,    mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       LOGS_NAME_COLUMN,    mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex,       LOGS_MESSAGE_COLUMN, mMessageTableWidgetItem);
    // clang-format on
}

LogsTableRecord::~LogsTableRecord()
{
    qDebug() << "Destroy LogsTableRecord";
}

void LogsTableRecord::setLogEntry(const LogEntry& entry)
{
    const QMutexLocker lock(mInstrumentsStorage->getMutex());

    const Instruments& instruments = mInstrumentsStorage->getInstruments();
    Instrument         instrument  = instruments[entry.instrumentId];

    const QIcon instrumentLogo(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), entry.instrumentId));

    if (instrument.ticker == "" || instrument.name == "")
    {
        instrument.ticker         = entry.instrumentId;
        instrument.name           = "?????";
        instrument.pricePrecision = 2;
    }

    mTimeTableWidgetItem->setValue(QDateTime::fromMSecsSinceEpoch(entry.timestamp));
    mLevelTableWidgetItem->setLogLevel(entry.level);
    mInstrumentTableItemWidget->setInstrumentLogo(instrumentLogo);
    mInstrumentTableItemWidget->setTicker(instrument.ticker);
    mInstrumentTableItemWidget->setFullText(instrument.name);
    mMessageTableWidgetItem->setText(entry.message);
}

void LogsTableRecord::filter(QTableWidget* tableWidget, LogLevel level)
{
    const int  row    = mTimeTableWidgetItem->row();
    const bool hidden = mLevelTableWidgetItem->logLevel() < level;

    tableWidget->setRowHidden(row, hidden);
}

void LogsTableRecord::exportToExcel(QXlsx::Document& doc) const
{
    const int row = mTimeTableWidgetItem->row() + 2; // Header and start index from 1

    QXlsx::Format cellStyle;
    cellStyle.setFillPattern(QXlsx::Format::PatternSolid);
    cellStyle.setBorderStyle(QXlsx::Format::BorderThin);
    cellStyle.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    cellStyle.setFontColor(CELL_FONT_COLOR);

    QXlsx::Format dateFormat;
    dateFormat.setNumberFormat(DATETIME_FORMAT);
    dateFormat.setFillPattern(QXlsx::Format::PatternSolid);
    dateFormat.setBorderStyle(QXlsx::Format::BorderThin);
    dateFormat.setPatternBackgroundColor(CELL_BACKGROUND_COLOR);
    dateFormat.setFontColor(CELL_FONT_COLOR);

    // clang-format off
    doc.write(row, LOGS_TIME_COLUMN + 1,    mTimeTableWidgetItem->value(), dateFormat);
    doc.write(row, LOGS_LEVEL_COLUMN + 1,   LOG_LEVEL_NAMES[mLevelTableWidgetItem->logLevel()], cellStyle);
    doc.write(row, LOGS_NAME_COLUMN + 1,    mInstrumentTableItemWidget->fullText(), cellStyle);
    doc.write(row, LOGS_MESSAGE_COLUMN + 1, mMessageTableWidgetItem->text(), cellStyle);
    // clang-format on
}
