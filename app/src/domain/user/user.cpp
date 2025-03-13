#include "src/domain/user/user.h"

#include <QMap>



QMap<QString, float> tariffToCommission{
    {"fees",    0.3f },
    {"trader",  0.05f},
    {"premium", 0.04f},
    {"sandbox", 0.04f}
};



User::User() :
    token(),
    qualified(),
    qualifiedForWorkWith(),
    tariff(),
    commission()
{
}

User::User(const User& another) :
    token(another.token),
    qualified(another.qualified),
    qualifiedForWorkWith(another.qualifiedForWorkWith),
    tariff(another.tariff),
    commission(another.commission)
{
}

User::~User()
{
}

User& User::operator=(const User& another)
{
    token                = another.token;
    qualified            = another.qualified;
    qualifiedForWorkWith = another.qualifiedForWorkWith;
    tariff               = another.tariff;
    commission           = another.commission;

    return *this;
}

void User::setTariff(const QString& value)
{
    tariff     = value;
    commission = tariffToCommission[tariff];
}

bool operator==(const User& lhs, const User& rhs)
{
    return lhs.token == rhs.token && lhs.qualified == rhs.qualified && lhs.qualifiedForWorkWith == rhs.qualifiedForWorkWith &&
           lhs.tariff == rhs.tariff && lhs.commission == rhs.commission;
}
