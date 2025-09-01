#include "src/utils/tradeutils/tradeutils.h"

#include <QDebug>



constexpr float HUNDRED_PERCENT = 100.0f;



TradeUtils::TradeUtils() :
    ITradeUtils()
{
    qDebug() << "Create TradeUtils";
}

TradeUtils::~TradeUtils()
{
    qDebug() << "Destroy TradeUtils";
}

qint64 TradeUtils::calculateAmountOfLotsToBuy(
    IConfig* config, double money, double totalCost, double turnover, double lotPrice, double lotPriceWithCommission
) const
{
    qint64 res;

    if (config->isLimitStockPurchase())
    {
        double cost = 0.0;

        if (config->isLimitByTurnover())
        {
            const double limitStockPurchasePart = config->getLimitStockPurchasePart() / HUNDRED_PERCENT;
            const double limitByTurnoverPercent = config->getLimitByTurnoverPercent() / HUNDRED_PERCENT;

            cost = qMin(totalCost * limitStockPurchasePart, turnover * limitByTurnoverPercent);
        }
        else
        {
            const double limitStockPurchasePart = config->getLimitStockPurchasePart() / HUNDRED_PERCENT;

            cost = totalCost * limitStockPurchasePart;
        }

        cost = qMax(cost, lotPrice);
        res  = qMin(qRound64(cost / lotPrice), static_cast<qint64>(money / lotPriceWithCommission));
    }
    else
    {
        res = money / lotPriceWithCommission;
    }

    return res;
}
