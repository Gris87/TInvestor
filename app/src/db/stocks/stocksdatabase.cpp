#include "stocksdatabase.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>



const int amountOfDataPerDay = 24 * 60;



StocksDatabase::StocksDatabase() :
    IStocksDatabase(),
    db(QSqlDatabase::addDatabase("QSQLITE"))
{
    qDebug() << "Create StocksDatabase";

    QString appDir = qApp->applicationDirPath();

    bool ok = QDir().mkpath(appDir + "/data/db");
    Q_ASSERT_X(ok, "StocksDatabase", "Failed to create dir");

    QString dbPath = appDir + "/data/db/stocks.db";
    db.setDatabaseName(dbPath);

    ok = db.open();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

    qInfo() << "Stocks database" << dbPath << "created";

    createListTable();
    // fillWithTestData();
}

StocksDatabase::~StocksDatabase()
{
    qDebug() << "Destroy StocksDatabase";

    db.close();
}

void StocksDatabase::createListTable()
{
    qDebug() << "Create table with list of stocks";

    QString str = "CREATE TABLE IF NOT EXISTS stocks ("
                  "    id       INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "    name     VARCHAR(255), "
                  "    fullname VARCHAR(255)"
                  ");";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
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

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
}

QList<Stock> StocksDatabase::readStocks()
{
    qDebug() << "Reading stocks from database";

    QString str = "SELECT name, fullname FROM stocks ORDER BY name;";

    QSqlQuery query(db);

    bool ok = query.exec(str);
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

    QSqlRecord rec = query.record();

    int nameIndex     = rec.indexOf("name");
    int fullnameIndex = rec.indexOf("fullname");

    QList<Stock> res;

    while (query.next())
    {
        Stock stock;

        stock.name     = query.value(nameIndex).toString();
        stock.fullname = query.value(fullnameIndex).toString();

        res.append(stock);

        qDebug() << "Read stock" << stock.name << ":" << stock.fullname;
    }

    return res;
}

void StocksDatabase::readStocksData(QList<Stock> &stocks)
{
    qDebug() << "Reading stocks dstatus from database";

    bool ok = db.transaction();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

    for (int i = 0; i < stocks.size(); ++i)
    {
        Stock &stock = stocks[i];

        QString str = QString("SELECT timestamp, value FROM %1;").arg(stock.name);

        QSqlQuery query(db);

        ok = query.exec(str);
        Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

        QSqlRecord rec = query.record();

        int timestampIndex = rec.indexOf("timestamp");
        int valueIndex     = rec.indexOf("value");

        int dataSize = query.last() ? query.at() + 1 : 0;
        int j = dataSize - 1;

        stock.data.reserve(dataSize + amountOfDataPerDay);
        stock.data.resize(dataSize);

        while (j >= 0)
        {
            StockData &stockData = stock.data[j];

            stockData.timestamp = query.value(timestampIndex).toLongLong();
            stockData.value     = query.value(valueIndex).toFloat();

            query.previous();
            --j;
        }

        qDebug() << "Read stock data" << stock.name;
    }

    ok = db.commit();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
}

void StocksDatabase::deleteObsoleteData(qint64 obsoleteTimestamp, const QList<Stock> &stocks)
{
    qDebug() << "Deleting obsolete stocks data";

    bool ok = db.transaction();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

    for (int i = 0; i < stocks.size(); ++i)
    {
        QString str = QString("DELETE FROM %1 WHERE timestamp < %2;").arg(stocks.at(i).name).arg(obsoleteTimestamp);

        QSqlQuery query(db);

        ok = query.exec(str);
        Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
    }

    ok = db.commit();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
}

void StocksDatabase::fillWithTestData() // TODO: Remove me
{
    bool ok = db.transaction();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

    QVariantList names;
    QVariantList fullnames;

    for (int i = 0; i < 100; ++i)
    {
        qInfo() << i;

        QString stockName = QString("AZAZ%1").arg(i);
        createStockTable(stockName);

        names << stockName;
        fullnames << "BLAH";
    }

    QSqlQuery query(db);

    query.prepare("INSERT INTO stocks (name, fullname) VALUES (?, ?);");
    query.addBindValue(names);
    query.addBindValue(fullnames);

    ok = !query.execBatch();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());

    for (int i = 0; i < 100; ++i)
    {
        qInfo() << i;

        QVariantList timestamps;
        QVariantList values;

        for (int j = 0; j < 2 * 365 * 24 * 60; ++j)
        {
            timestamps << 1672520400 + j * 60;
            values << i * j;
        }

        QSqlQuery query(db);

        query.prepare(QString("INSERT INTO AZAZ%1 (timestamp, value) VALUES (?, ?);").arg(i));
        query.addBindValue(timestamps);
        query.addBindValue(values);

        ok = query.execBatch();
        Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
    }

    ok = db.commit();
    Q_ASSERT_X(ok, "StocksDatabase", db.lastError().text().toLocal8Bit().constData());
}
