#pragma once



#include "src/config/iconfig.h"
#include "src/domain/portfolio/portfolio.h"
#include "src/domain/stock/stock.h"
#include "src/domain/trading/tradinginfo.h"



using InstrumentSells = QMap<QString, qint64>; // UID => Timestamp



class IDecisionMaker
{
public:
    IDecisionMaker()          = default;
    virtual ~IDecisionMaker() = default;

    IDecisionMaker(const IDecisionMaker& another)            = delete;
    IDecisionMaker& operator=(const IDecisionMaker& another) = delete;

    virtual InstrumentsForTrading makeDecision(
        QThread*               parentThread,
        qint64                 timestamp,
        IConfig*               config,
        const InstrumentSells& instrumentSells,
        const Portfolio&       portfolio,
        const QList<Stock*>&   stocks,
        bool                   autoPilot,
        int                    keepMoney,
        bool                   dateRange,
        bool                   useParallel
    ) = 0;
};
