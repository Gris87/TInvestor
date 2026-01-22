#include "src/utils/tradeutils/tradeutils.h"

#include <QDebug>



constexpr float HUNDRED_PERCENT = 100.0f;



TradeUtils::TradeUtils(ITimeUtils* timeUtils) :
    ITradeUtils(),
    mTimeUtils(timeUtils)
{
    qDebug() << "Create TradeUtils";
}

TradeUtils::~TradeUtils()
{
    qDebug() << "Destroy TradeUtils";
}

qint64 TradeUtils::calculateAmountOfLotsToBuy(
    IConfig* config,
    qint64   timestamp,
    double   money,
    double   totalCost,
    double   turnover,
    double   lotPrice,
    double   lotPriceWithCommission
) const
{
    bool   limitStockPurchase     = false;
    double limitStockPurchasePart = 0.0;
    bool   limitByTurnover        = false;
    double limitByTurnoverPercent = 0.0;

    if (config->isLimitStockPurchaseNonWorkingHours() && !mTimeUtils->isWorkingHours(timestamp))
    {
        limitStockPurchase     = true;
        limitStockPurchasePart = config->getLimitStockPurchasePartNonWorkingHours();

        if (config->isLimitByTurnoverNonWorkingHours())
        {
            limitByTurnover        = true;
            limitByTurnoverPercent = config->getLimitByTurnoverPercentNonWorkingHours();
        }
        else
        {
            limitByTurnover        = config->isLimitByTurnover();
            limitByTurnoverPercent = config->getLimitByTurnoverPercent();
        }
    }
    else
    {
        limitStockPurchase     = config->isLimitStockPurchase();
        limitStockPurchasePart = config->getLimitStockPurchasePart();
        limitByTurnover        = config->isLimitByTurnover();
        limitByTurnoverPercent = config->getLimitByTurnoverPercent();
    }

    return calculateAmountOfLotsToBuy(
        limitStockPurchase,
        limitStockPurchasePart,
        limitByTurnover,
        limitByTurnoverPercent,
        money,
        totalCost,
        turnover,
        lotPrice,
        lotPriceWithCommission
    );
}

qint64 TradeUtils::calculateAmountOfLotsToBuy(
    bool   limitStockPurchase,
    double limitStockPurchasePart,
    bool   limitByTurnover,
    double limitByTurnoverPercent,
    double money,
    double totalCost,
    double turnover,
    double lotPrice,
    double lotPriceWithCommission
) const
{
    qint64 res = 0;

    if (limitStockPurchase)
    {
        double cost = totalCost * limitStockPurchasePart / HUNDRED_PERCENT;

        if (limitByTurnover)
        {
            cost = qMin(cost, turnover * limitByTurnoverPercent / HUNDRED_PERCENT);
        }

        cost = qMax(cost, lotPrice);
        res  = qMin(qRound64(cost / lotPrice), static_cast<qint64>(money / lotPriceWithCommission));
    }
    else
    {
        res = money / lotPriceWithCommission;
    }

    // TODO: Remove it
    if (res == 1)
    {
        qInfo() << "AAAAAAAAAAAAAAA" << limitStockPurchase << limitStockPurchasePart << limitByTurnover << limitByTurnoverPercent
                << money << totalCost << turnover << lotPrice << lotPriceWithCommission;
    }

    return res;
}
