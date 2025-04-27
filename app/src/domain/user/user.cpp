#include "src/domain/user/user.h"

#include <QMap>



const char* SANDBOX_TOKEN = "t.dFIbMnfNHi4EGR17LdlVerWmcQ53eNFvSYJqJKKXyfOfvLNLizHULt_fUPItm2Y9-jeuWs01KzlPk8dXoGonAQ";

static const QMap<QString, float> TARIFF_TO_COMMISSION{
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
