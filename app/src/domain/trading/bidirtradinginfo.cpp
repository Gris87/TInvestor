#include "src/domain/trading/bidirtradinginfo.h"



BiDirTradingInfo::BiDirTradingInfo() :
    cause()
{
}

BiDirTradingInfo::BiDirTradingInfo(const QString& _cause) :
    cause(_cause)
{
}
