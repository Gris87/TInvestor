#ifndef STOCKSDATABASE_H
#define STOCKSDATABASE_H

#include <QObject>

#include <QtSql>



class StocksDatabase : public QObject
{
    Q_OBJECT

public:
    explicit StocksDatabase(QObject *parent = nullptr);
    ~StocksDatabase();

private:
    void createListTable();

    QSqlDatabase mDB;
};



#endif // STOCKSDATABASE_H
