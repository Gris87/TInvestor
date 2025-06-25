#pragma once



#include <Qt>



enum UserRoles : qint16
{
    ROLE_INSTRUMENT_LOGO   = Qt::UserRole + 1,
    ROLE_INSTRUMENT_NAME   = Qt::UserRole + 2,
    ROLE_INSTRUMENT_LOCKED = Qt::UserRole + 3,
    ROLE_STOCK             = Qt::UserRole + 4,
};
