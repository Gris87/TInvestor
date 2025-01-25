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

    void createStockTable(const QString &name);

private:
    void createListTable();
    void fillWithTestData();

    QSqlDatabase mDB;
};



#endif // STOCKSDATABASE_H
