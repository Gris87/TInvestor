#pragma once



#include <QObject>

#include <QList>
#include <QtSql>

#include "src/domain/stocks/stock.h"



class StocksDatabase : public QObject
{
    Q_OBJECT

public:
    explicit StocksDatabase(QObject *parent = nullptr);
    ~StocksDatabase();

    void createStockTable(const QString &name);
    void readStocks(QList<Stock> *stocks);
    void deleteObsoleteData(qint64 obsoleteTimestamp, const QList<Stock> &stocks);

private:
    void createListTable();
    void fillWithTestData();

    QSqlDatabase mDB;
};
