#pragma once



#include <QString>



struct Account
{
    Account();
    Account(const Account& another);
    ~Account();

    Account& operator=(const Account& another);

    void setId(const QString& value);

    QString id;
    QString idHash;
    QString name;
};



bool operator==(const Account& lhs, const Account& rhs);
