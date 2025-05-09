#include "src/domain/user/account.h"

#include <QCryptographicHash>



Account::Account() :
    index(),
    id(),
    name()
{
}

QString Account::hash() const
{
    return QString::fromUtf8(QCryptographicHash::hash(id.toUtf8(), QCryptographicHash::Md5).toHex());
}

bool operator==(const Account& lhs, const Account& rhs)
{
    return lhs.index == rhs.index && lhs.id == rhs.id && lhs.name == rhs.name;
}

bool operator<(const Account& lhs, const Account& rhs)
{
    return lhs.index < rhs.index;
}
