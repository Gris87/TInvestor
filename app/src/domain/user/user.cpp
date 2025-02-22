#include "src/domain/user/user.h"

#include <QCryptographicHash>



QMap<QString, float> tariffToCommission{
    {"fees",    0.3f },
    {"trader",  0.05f},
    {"premium", 0.04f},
    {"sandbox", 0.04f}
};



void User::setTariff(const QString& value)
{
    tariff     = value;
    commission = tariffToCommission[tariff];
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
