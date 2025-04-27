#pragma once



#include <QStringList>



extern const char* SANDBOX_TOKEN;



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
