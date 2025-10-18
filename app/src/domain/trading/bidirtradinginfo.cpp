#include "src/domain/trading/bidirtradinginfo.h"



BiDirTradingInfo::BiDirTradingInfo() :
    stock(),
    mode(),
    cause()
{
}

BiDirTradingInfo::BiDirTradingInfo(Stock* _stock, BiDirMode _mode, const QString& _cause) :
    stock(_stock),
    mode(_mode),
    cause(_cause)
{
}
