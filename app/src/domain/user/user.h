#pragma once



#include <QMap>
#include <QStringList>



#define SANDBOX_TOKEN "t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ"



struct User
{
    User();
    User(const User& another);
    ~User();

    User& operator=(const User& another);

    void setTariff(const QString& value);

    QString     token;
    bool        qualified;
    QStringList qualifiedForWorkWith;
    QString     tariff;
    float       commission;
};

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
