#include "src/widgets/tablerecords/operationstablerecord/operationstablerecord.h"

#include <QCoreApplication>
#include <QDebug>



OperationsTableRecord::OperationsTableRecord(
    QTableWidget*                      tableWidget,
    IInstrumentTableItemWidgetFactory* instrumentTableItemWidgetFactory,
    IUserStorage*                      userStorage,
    IInstrumentsStorage*               instrumentsStorage,
    QObject*                           parent
) :
    IOperationsTableRecord(parent),
    mTimeTableWidgetItem(new TimeTableItem()),
    mInstrumentTableItemWidget(),
    mInstrumentsStorage(instrumentsStorage)
{
    qDebug() << "Create OperationsTableRecord";

    mInstrumentTableItemWidget =
        instrumentTableItemWidgetFactory->newInstance(userStorage, tableWidget); // tableWidget will take ownership

    const int rowIndex = tableWidget->rowCount();
    tableWidget->setRowCount(rowIndex + 1);

    tableWidget->setItem(rowIndex, OPERATIONS_TIME_COLUMN, mTimeTableWidgetItem);
    tableWidget->setCellWidget(rowIndex, OPERATIONS_NAME_COLUMN, mInstrumentTableItemWidget);
    tableWidget->setItem(rowIndex, OPERATIONS_NAME_COLUMN, mInstrumentTableItemWidget);
}

OperationsTableRecord::~OperationsTableRecord()
{
    qDebug() << "Destroy OperationsTableRecord";
}

void OperationsTableRecord::setOperation(const Operation& operation)
{
    const QMutexLocker lock(mInstrumentsStorage->getMutex());

    const Instruments& instruments = mInstrumentsStorage->getInstruments();
    Instrument         instrument  = instruments[operation.instrumentId];

    const QIcon instrumentLogo(QString("%1/data/instruments/logos/%2.png").arg(qApp->applicationDirPath(), operation.instrumentId)
    );

    if (instrument.ticker == "")
    {
        instrument.ticker = operation.instrumentId;
    }

    if (instrument.name == "")
    {
        instrument.name = "?????";
    }

    mTimeTableWidgetItem->setValue(QDateTime::fromMSecsSinceEpoch(operation.timestamp));
    mInstrumentTableItemWidget->setIcon(instrumentLogo);
    mInstrumentTableItemWidget->setText(instrument.ticker);
    mInstrumentTableItemWidget->setFullText(instrument.name);
}
