#include "src/domain/user/user.h"

#include <QMap>



// NOLINTBEGIN(readability-magic-numbers)
const QMap<QString, float> TARIFF_TO_COMMISSION{
    {"fees",    0.3f },
    {"trader",  0.05f},
    {"premium", 0.04f},
    {"sandbox", 0.04f}
};
// NOLINTEND(readability-magic-numbers)



User::User() :
    token(),
    qualified(),
    qualifiedForWorkWith(),
    tariff(),
    commission()
{
}

void User::setTariff(const QString& value)
{
    tariff     = value;
    commission = TARIFF_TO_COMMISSION[tariff];
}

bool operator==(const User& lhs, const User& rhs)
{
    return lhs.token == rhs.token && lhs.qualified == rhs.qualified && lhs.qualifiedForWorkWith == rhs.qualifiedForWorkWith &&
           lhs.tariff == rhs.tariff && lhs.commission == rhs.commission;
}
