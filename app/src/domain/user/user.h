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
};

struct Account
{
    QString id;
    QString idHash;
    QString name;
};



bool operator==(const Account& lhs, const Account& rhs);



extern QMap<QString, float> tariffToCommission;
