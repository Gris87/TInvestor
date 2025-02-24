#include "src/domain/user/user.h"

#include <QCryptographicHash>



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

Account::Account() :
    id(),
    idHash(),
    name()
{
}

Account::Account(const Account& another) :
    id(another.id),
    idHash(another.idHash),
    name(another.name)
{
}

Account::~Account()
{
}

Account& Account::operator=(const Account& another)
{
    id     = another.id;
    idHash = another.idHash;
    name   = another.name;

    return *this;
}

void Account::setId(const QString& value)
{
    id     = value;
    idHash = QString::fromUtf8(QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Md5).toHex());
}

bool operator==(const Account& lhs, const Account& rhs)
{
    return lhs.id == rhs.id && lhs.name == rhs.name;
}
