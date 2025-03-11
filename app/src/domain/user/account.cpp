#include "src/domain/user/account.h"

#include <QCryptographicHash>



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
