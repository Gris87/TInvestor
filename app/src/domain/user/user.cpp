#include "src/domain/user/user.h"



QMap<QString, float> tariffToCommission{
    {"fees",    0.3f },
    {"trader",  0.05f},
    {"premium", 0.04f},
    {"sandbox", 0.04f}
};



bool operator==(const Account& lhs, const Account& rhs)
{
    return lhs.id == rhs.id && lhs.name == rhs.name;
}
