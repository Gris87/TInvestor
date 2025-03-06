#pragma once



#include "src/widgets/tablerecord/itablerecord.h"

#include <QTableWidget>

#include "src/domain/stocks/stocks.h"



class TableRecord : public ITableRecord
{
    Q_OBJECT

public:
    explicit TableRecord(QTableWidget* tableWidget, Stock* stock, QObject* parent = nullptr);
    ~TableRecord();

    TableRecord(const TableRecord& another)            = delete;
    TableRecord& operator=(const TableRecord& another) = delete;

    void updateAll() override;
    void updatePrice() override;

private:
    Stock*            mStock;
    QTableWidgetItem* mStockTableWidgetItem;
    QTableWidgetItem* mPriceTableWidgetItem;
    QTableWidgetItem* mDayChangeTableWidgetItem;
    QTableWidgetItem* mDateChangeTableWidgetItem;
    QTableWidgetItem* mPaybackTableWidgetItem;
    QTableWidgetItem* mLinkTableWidgetItem;
};
