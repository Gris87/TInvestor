#pragma once



#include <QThread>

#include "src/config/decisions/idecisionmakerconfig.h"
#include "src/domain/stock/stock.h"



class IActionDecision
{
public:
    IActionDecision()          = default;
    virtual ~IActionDecision() = default;

    IActionDecision(const IActionDecision& another)            = delete;
    IActionDecision& operator=(const IActionDecision& another) = delete;

    virtual QString makeDecision(
        QThread*              parentThread,
        IDecisionMakerConfig* config,
        qint64                limitTimestamp,
        Stock*                stock,
        bool                  dateRange,
        int                   dataIndex,
        float                 price,
        float                 avgPrice,
        float                 commission
    ) = 0;

    [[nodiscard]]
    virtual bool isAsap() const = 0;
};
