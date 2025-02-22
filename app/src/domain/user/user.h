#pragma once



#include <QMap>
#include <QStringList>



#define SANDBOX_TOKEN "t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ"



struct User
{
    QString     token;
    bool        qualified;
    QStringList qualifiedForWorkWith;
    QString     tariff;
    float       commission;

    void setTariff(const QString& value);
};

struct Account
{
    QString id;
    QString idHash;
    QString name;

    void setId(const QString& value);
};



bool operator==(const Account& lhs, const Account& rhs);
