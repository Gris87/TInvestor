#pragma once



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
