#pragma once



#include <QString>



struct Account
{
    Account();
    Account(const Account& another) = default;
    ~Account()                      = default;

    Account& operator=(const Account& another) = default;

    void setId(const QString& value);

    QString id;
    QString idHash;
    QString name;
};



bool operator==(const Account& lhs, const Account& rhs);
