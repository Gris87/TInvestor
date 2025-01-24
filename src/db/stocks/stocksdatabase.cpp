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

    if (!mDB.open()) {
        qFatal() << "Failed to open database";
    }

    qInfo() << "Stocks database" << dbPath << "created";

    createListTable();
}

StocksDatabase::~StocksDatabase()
{
    qDebug() << "Destroy StocksDatabase";
}

void StocksDatabase::createListTable()
{
    qDebug() << "Create table with list of stocks";

    QString str = "CREATE TABLE IF NOT EXISTS stocks ("
                  "    id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "    name VARCHAR(255)"
                  ");";

    QSqlQuery query;

    if (!query.exec(str)) {
        qFatal() << "Failed to create stocks table";
    }
}
