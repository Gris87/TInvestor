#pragma once



#include "src/config/iconfig.h"
#include "src/domain/portfolio/portfolio.h"
#include "src/domain/stock/stock.h"
#include "src/domain/trading/tradinginfo.h"



class IDecisionMaker
{
public:
    IDecisionMaker()          = default;
    virtual ~IDecisionMaker() = default;

    IDecisionMaker(const IDecisionMaker& another)            = delete;
    IDecisionMaker& operator=(const IDecisionMaker& another) = delete;

    virtual InstrumentsForTrading makeDecision(
        QThread*             parentThread,
        qint64               timestamp,
        IConfig*             config,
        const Portfolio&     portfolio,
        const QList<Stock*>& stocks,
        bool                 autoPilot,
        int                  keepMoney,
        bool                 dateRange
    ) = 0;
};
