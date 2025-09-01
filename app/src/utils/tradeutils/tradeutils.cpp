#include "src/utils/tradeutils/tradeutils.h"

#include <QDebug>



TradeUtils::TradeUtils() :
    ITradeUtils()
{
    qDebug() << "Create TradeUtils";
}

TradeUtils::~TradeUtils()
{
    qDebug() << "Destroy TradeUtils";
}
