#pragma once



#include <QStringList>



const QString SANDBOX_TOKEN = "t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ";



struct User
{
    User();
    User(const User& another) = default;
    ~User()                   = default;

    User& operator=(const User& another) = default;

    void setTariff(const QString& value);

    QString     token;
    bool        qualified;
    QStringList qualifiedForWorkWith;
    QString     tariff;
    float       commission;
};



bool operator==(const User& lhs, const User& rhs);
