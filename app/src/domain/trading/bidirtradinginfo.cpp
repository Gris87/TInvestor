#include "src/domain/trading/bidirtradinginfo.h"



BiDirTradingInfo::BiDirTradingInfo() :
    turnover(),
    cause()
{
}

BiDirTradingInfo::BiDirTradingInfo(qint64 _turnover, const QString& _cause) :
    turnover(_turnover),
    cause(_cause)
{
}
