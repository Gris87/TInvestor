#pragma once



#include <QMap>



struct Account
{
    Account();
    Account(const Account& another) = default;
    ~Account()                      = default;

    Account& operator=(const Account& another) = default;

    QString hash() const;

    int     index;
    QString id;
    QString name;
};

using Accounts = QMap<QString, Account>; // Hash => Account



bool operator==(const Account& lhs, const Account& rhs);
bool operator<(const Account& lhs, const Account& rhs);
