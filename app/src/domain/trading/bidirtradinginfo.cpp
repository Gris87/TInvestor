#include "src/domain/trading/bidirtradinginfo.h"



BiDirTradingInfo::BiDirTradingInfo() :
    stock(),
    cause()
{
}

BiDirTradingInfo::BiDirTradingInfo(Stock* _stock, const QString& _cause) :
    stock(_stock),
    cause(_cause)
{
}
