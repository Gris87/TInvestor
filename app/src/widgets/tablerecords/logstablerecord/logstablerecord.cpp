#include "src/widgets/tablerecords/logstablerecord/logstablerecord.h"

#include <QCoreApplication>
#include <QDebug>



const char* const DATETIME_FORMAT       = "yyyy-MM-dd hh:mm:ss";
const QColor      CELL_BACKGROUND_COLOR = QColor("#2C3C4B"); // clazy:exclude=non-pod-global-static
const QColor      CELL_FONT_COLOR       = QColor("#97AEC4"); // clazy:exclude=non-pod-global-static



LogsTableRecord::LogsTableRecord(QTableWidget* tableWidget, QObject* parent) :
    ILogsTableRecord(parent),
    mTimeTableWidgetItem(new TimeTableItem()),
    mLevelTableWidgetItem(new QTableWidgetItem()),
    mMessageTableWidgetItem(new QTableWidgetItem())
{
    qDebug() << "Create LogsTableRecord";

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    // clang-format off
    tableWidget->setItem(rowIndex, LOGS_TIME_COLUMN,    mTimeTableWidgetItem);
    tableWidget->setItem(rowIndex, LOGS_LEVEL_COLUMN,   mLevelTableWidgetItem);
    tableWidget->setItem(rowIndex, LOGS_MESSAGE_COLUMN, mMessageTableWidgetItem);
    // clang-format on
}

LogsTableRecord::~LogsTableRecord()
{
    qDebug() << "Destroy LogsTableRecord";
}

void LogsTableRecord::setLogEntry(const LogEntry& entry)
{
    mTimeTableWidgetItem->setValue(QDateTime::fromMSecsSinceEpoch(entry.timestamp));
    mLevelTableWidgetItem->setIcon(QIcon(QString(":/assets/images/levels/%1.png").arg(LOG_LEVEL_NAMES_LOWERCASE[entry.level])));
    mLevelTableWidgetItem->setData(Qt::UserRole, entry.level);
    mMessageTableWidgetItem->setText(entry.message);
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
    doc.write(row, LOGS_LEVEL_COLUMN + 1,   LOG_LEVEL_NAMES[mLevelTableWidgetItem->data(Qt::UserRole).toInt()], cellStyle);
    doc.write(row, LOGS_MESSAGE_COLUMN + 1, mMessageTableWidgetItem->text(), cellStyle);
    // clang-format on
}
