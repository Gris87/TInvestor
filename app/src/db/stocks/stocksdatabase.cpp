#include "stocksdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



StocksDatabase::StocksDatabase(QObject *parent) :
    QObject(parent),
    mDB(QSqlDatabase::addDatabase("QSQLITE"))
{
    qDebug() << "Create StocksDatabase";

    QString appDir = qApp->applicationDirPath();

    if (!QDir().mkpath(appDir + "/data/db"))
    {
        qFatal() << "Failed to create dir";
    }

    QString dbPath = appDir + "/data/db/stocks.db";
    mDB.setDatabaseName(dbPath);

    if (!mDB.open())
    {
        qFatal() << "Failed to open database";
    }

    qInfo() << "Stocks database" << dbPath << "created";

    createListTable();
    // fillWithTestData();
}

StocksDatabase::~StocksDatabase()
{
    qDebug() << "Destroy StocksDatabase";
}

void StocksDatabase::createListTable()
{
    qDebug() << "Create table with list of stocks";

    QString str = "CREATE TABLE IF NOT EXISTS stocks ("
                  "    id       INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "    name     VARCHAR(255), "
                  "    fullname VARCHAR(255)"
                  ");";

    QSqlQuery query(mDB);

    if (!query.exec(str))
    {
        qFatal() << "Failed to create stocks table:" << mDB.lastError().text();
    }
}

void StocksDatabase::createStockTable(const QString &name)
{
    qDebug() << "Create table:" << name;

    QString str = "CREATE TABLE IF NOT EXISTS %1 ("
                  "    id        INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "    timestamp INTEGER, "
                  "    value     REAL"
                  ");";

    str = str.arg(name);

    QSqlQuery query(mDB);

    if (!query.exec(str))
    {
        qFatal() << "Failed to create" << name << "table:" << mDB.lastError().text();
    }
}

void StocksDatabase::readStocks(QList<Stock> *stocks)
{
    qDebug() << "Reading stocks from database";

    QString str = "SELECT name, fullname FROM stocks ORDER BY name;";

    QSqlQuery query(mDB);

    if (!query.exec(str))
    {
        qFatal() << "Failed to read stocks:" << mDB.lastError().text();
    }

    QSqlRecord rec = query.record();

    int nameIndex     = rec.indexOf("name");
    int fullnameIndex = rec.indexOf("fullname");

    while (query.next())
    {
        Stock stock;

        stock.name     = query.value(nameIndex).toString();
        stock.fullname = query.value(fullnameIndex).toString();

        stocks->append(stock);

        qDebug() << "Read stock" << stock.name << ":" << stock.fullname;
    }
}

void StocksDatabase::deleteObsoleteData(qint64 obsoleteTimestamp, const QList<Stock> &stocks)
{
    qDebug() << "Deleting obsolete stocks data";

    if (!mDB.transaction())
    {
        qFatal() << "Failed to start transaction" << mDB.lastError().text();
    }

    for (int i = 0; i < stocks.size(); ++i)
    {
        QString str = QString("DELETE FROM %1 WHERE timestamp < %2;").arg(stocks.at(i).name).arg(obsoleteTimestamp);

        QSqlQuery query(mDB);

        if (!query.exec(str))
        {
            qFatal() << "Failed to read stocks:" << mDB.lastError().text();
        }
    }

    if (!mDB.commit())
    {
        qFatal() << "Failed to commit transaction" << mDB.lastError().text();
    }
}

void StocksDatabase::fillWithTestData() // TODO: Remove me
{
    if (!mDB.transaction())
    {
        qFatal() << "Failed to start transaction" << mDB.lastError().text();
    }

    QVariantList names;
    QVariantList fullnames;

    for (int i = 0; i < 100; ++i) {
        qInfo() << i;

        QString stockName = QString("AZAZ%1").arg(i);
        createStockTable(stockName);

        names << stockName;
        fullnames << "BLAH";
    }

    QSqlQuery query(mDB);

    query.prepare("INSERT INTO stocks (name, fullname) VALUES (?, ?);");
    query.addBindValue(names);
    query.addBindValue(fullnames);

    if (!query.execBatch())
    {
        qFatal() << "Failed to insert to stocks table:" << mDB.lastError().text();
    }

    for (int i = 0; i < 100; ++i) {
        qInfo() << i;

        QVariantList timestamps;
        QVariantList values;

        for (int j = 0; j < 2 * 365 * 24 * 60; ++j) {
            timestamps << 1672520400 + j * 60;
            values << i * j;
        }

        QSqlQuery query(mDB);

        query.prepare(QString("INSERT INTO AZAZ%1 (timestamp, value) VALUES (?, ?);").arg(i));
        query.addBindValue(timestamps);
        query.addBindValue(values);

        if (!query.execBatch())
        {
            qFatal() << "Failed to insert stocks value:" << mDB.lastError().text();
        }
    }

    if (!mDB.commit())
    {
        qFatal() << "Failed to commit transaction" << mDB.lastError().text();
    }
}
