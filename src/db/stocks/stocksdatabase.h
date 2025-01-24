#ifndef STOCKSDATABASE_H
#define STOCKSDATABASE_H

#include <QObject>



class StocksDatabase : public QObject
{
    Q_OBJECT

public:
    explicit StocksDatabase(QObject *parent = nullptr);
    ~StocksDatabase();
};



#endif // STOCKSDATABASE_H
