#pragma once



#include "src/domain/operation/operation.h"



class OperationsTableTimeLessThan
{
public:
    explicit OperationsTableTimeLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).timestamp < entries->at(r).timestamp;
    }
};

class OperationsTableNameLessThan
{
public:
    explicit OperationsTableNameLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).instrumentTicker < entries->at(r).instrumentTicker;
    }
};

class OperationsTableDescriptionLessThan
{
public:
    explicit OperationsTableDescriptionLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).description < entries->at(r).description;
    }
};

class OperationsTablePriceLessThan
{
public:
    explicit OperationsTablePriceLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).price < entries->at(r).price;
    }
};

class OperationsTableAvgPriceFifoLessThan
{
public:
    explicit OperationsTableAvgPriceFifoLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).avgPriceFifo < entries->at(r).avgPriceFifo;
    }
};

class OperationsTableAvgPriceWavgLessThan
{
public:
    explicit OperationsTableAvgPriceWavgLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).avgPriceWavg < entries->at(r).avgPriceWavg;
    }
};

class OperationsTableQuantityLessThan
{
public:
    explicit OperationsTableQuantityLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).quantity < entries->at(r).quantity;
    }
};

class OperationsTableRemainedQuantityLessThan
{
public:
    explicit OperationsTableRemainedQuantityLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).remainedQuantity < entries->at(r).remainedQuantity;
    }
};

class OperationsTablePaymentLessThan
{
public:
    explicit OperationsTablePaymentLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).payment < entries->at(r).payment;
    }
};

class OperationsTableCommissionLessThan
{
public:
    explicit OperationsTableCommissionLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).commission < entries->at(r).commission;
    }
};

class OperationsTableYieldLessThan
{
public:
    explicit OperationsTableYieldLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).yield < entries->at(r).yield;
    }
};

class OperationsTableYieldWithCommissionLessThan
{
public:
    explicit OperationsTableYieldWithCommissionLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).yieldWithCommission < entries->at(r).yieldWithCommission;
    }
};

class OperationsTableYieldWithCommissionPercentLessThan
{
public:
    explicit OperationsTableYieldWithCommissionPercentLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).yieldWithCommissionPercent < entries->at(r).yieldWithCommissionPercent;
    }
};

class OperationsTableTotalYieldWithCommissionLessThan
{
public:
    explicit OperationsTableTotalYieldWithCommissionLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).totalYieldWithCommission < entries->at(r).totalYieldWithCommission;
    }
};

class OperationsTableTotalYieldWithCommissionPercentLessThan
{
public:
    explicit OperationsTableTotalYieldWithCommissionPercentLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).totalYieldWithCommissionPercent < entries->at(r).totalYieldWithCommissionPercent;
    }
};

class OperationsTableRemainedMoneyLessThan
{
public:
    explicit OperationsTableRemainedMoneyLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).remainedMoney < entries->at(r).remainedMoney;
    }
};

class OperationsTableTotalMoneyLessThan
{
public:
    explicit OperationsTableTotalMoneyLessThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).totalMoney < entries->at(r).totalMoney;
    }
};

class OperationsTableTimeGreaterThan
{
public:
    explicit OperationsTableTimeGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).timestamp > entries->at(r).timestamp;
    }
};

class OperationsTableNameGreaterThan
{
public:
    explicit OperationsTableNameGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).instrumentTicker > entries->at(r).instrumentTicker;
    }
};

class OperationsTableDescriptionGreaterThan
{
public:
    explicit OperationsTableDescriptionGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).description > entries->at(r).description;
    }
};

class OperationsTablePriceGreaterThan
{
public:
    explicit OperationsTablePriceGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).price > entries->at(r).price;
    }
};

class OperationsTableAvgPriceFifoGreaterThan
{
public:
    explicit OperationsTableAvgPriceFifoGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).avgPriceFifo > entries->at(r).avgPriceFifo;
    }
};

class OperationsTableAvgPriceWavgGreaterThan
{
public:
    explicit OperationsTableAvgPriceWavgGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).avgPriceWavg > entries->at(r).avgPriceWavg;
    }
};

class OperationsTableQuantityGreaterThan
{
public:
    explicit OperationsTableQuantityGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).quantity > entries->at(r).quantity;
    }
};

class OperationsTableRemainedQuantityGreaterThan
{
public:
    explicit OperationsTableRemainedQuantityGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).remainedQuantity > entries->at(r).remainedQuantity;
    }
};

class OperationsTablePaymentGreaterThan
{
public:
    explicit OperationsTablePaymentGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).payment > entries->at(r).payment;
    }
};

class OperationsTableCommissionGreaterThan
{
public:
    explicit OperationsTableCommissionGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).commission > entries->at(r).commission;
    }
};

class OperationsTableYieldGreaterThan
{
public:
    explicit OperationsTableYieldGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).yield > entries->at(r).yield;
    }
};

class OperationsTableYieldWithCommissionGreaterThan
{
public:
    explicit OperationsTableYieldWithCommissionGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).yieldWithCommission > entries->at(r).yieldWithCommission;
    }
};

class OperationsTableYieldWithCommissionPercentGreaterThan
{
public:
    explicit OperationsTableYieldWithCommissionPercentGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).yieldWithCommissionPercent > entries->at(r).yieldWithCommissionPercent;
    }
};

class OperationsTableTotalYieldWithCommissionGreaterThan
{
public:
    explicit OperationsTableTotalYieldWithCommissionGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).totalYieldWithCommission > entries->at(r).totalYieldWithCommission;
    }
};

class OperationsTableTotalYieldWithCommissionPercentGreaterThan
{
public:
    explicit OperationsTableTotalYieldWithCommissionPercentGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).totalYieldWithCommissionPercent > entries->at(r).totalYieldWithCommissionPercent;
    }
};

class OperationsTableRemainedMoneyGreaterThan
{
public:
    explicit OperationsTableRemainedMoneyGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).remainedMoney > entries->at(r).remainedMoney;
    }
};

class OperationsTableTotalMoneyGreaterThan
{
public:
    explicit OperationsTableTotalMoneyGreaterThan(QList<Operation>* _entries) :
        entries(_entries)
    {
    }

    QList<Operation>* entries;

    inline bool operator()(int l, int r) const
    {
        return entries->at(l).totalMoney > entries->at(r).totalMoney;
    }
};

inline bool operationsTimeLess(const Operation& l, const Operation& r)
{
    return l.timestamp < r.timestamp;
}

inline bool operationsNameLess(const Operation& l, const Operation& r)
{
    return l.instrumentTicker < r.instrumentTicker;
}

inline bool operationsDescriptionLess(const Operation& l, const Operation& r)
{
    return l.description < r.description;
}

inline bool operationsPriceLess(const Operation& l, const Operation& r)
{
    return l.price < r.price;
}

inline bool operationsAvgPriceFifoLess(const Operation& l, const Operation& r)
{
    return l.avgPriceFifo < r.avgPriceFifo;
}

inline bool operationsAvgPriceWavgLess(const Operation& l, const Operation& r)
{
    return l.avgPriceWavg < r.avgPriceWavg;
}

inline bool operationsQuantityLess(const Operation& l, const Operation& r)
{
    return l.quantity < r.quantity;
}

inline bool operationsRemainedQuantityLess(const Operation& l, const Operation& r)
{
    return l.remainedQuantity < r.remainedQuantity;
}

inline bool operationsPaymentLess(const Operation& l, const Operation& r)
{
    return l.payment < r.payment;
}

inline bool operationsCommissionLess(const Operation& l, const Operation& r)
{
    return l.commission < r.commission;
}

inline bool operationsYieldLess(const Operation& l, const Operation& r)
{
    return l.yield < r.yield;
}

inline bool operationsYieldWithCommissionLess(const Operation& l, const Operation& r)
{
    return l.yieldWithCommission < r.yieldWithCommission;
}

inline bool operationsYieldWithCommissionPercentLess(const Operation& l, const Operation& r)
{
    return l.yieldWithCommissionPercent < r.yieldWithCommissionPercent;
}

inline bool operationsTotalYieldWithCommissionLess(const Operation& l, const Operation& r)
{
    return l.totalYieldWithCommission < r.totalYieldWithCommission;
}

inline bool operationsTotalYieldWithCommissionPercentLess(const Operation& l, const Operation& r)
{
    return l.totalYieldWithCommissionPercent < r.totalYieldWithCommissionPercent;
}

inline bool operationsRemainedMoneyLess(const Operation& l, const Operation& r)
{
    return l.remainedMoney < r.remainedMoney;
}

inline bool operationsTotalMoneyLess(const Operation& l, const Operation& r)
{
    return l.totalMoney < r.totalMoney;
}

inline bool operationsTimeGreater(const Operation& l, const Operation& r)
{
    return l.timestamp > r.timestamp;
}

inline bool operationsNameGreater(const Operation& l, const Operation& r)
{
    return l.instrumentTicker > r.instrumentTicker;
}

inline bool operationsDescriptionGreater(const Operation& l, const Operation& r)
{
    return l.description > r.description;
}

inline bool operationsPriceGreater(const Operation& l, const Operation& r)
{
    return l.price > r.price;
}

inline bool operationsAvgPriceFifoGreater(const Operation& l, const Operation& r)
{
    return l.avgPriceFifo > r.avgPriceFifo;
}

inline bool operationsAvgPriceWavgGreater(const Operation& l, const Operation& r)
{
    return l.avgPriceWavg > r.avgPriceWavg;
}

inline bool operationsQuantityGreater(const Operation& l, const Operation& r)
{
    return l.quantity > r.quantity;
}

inline bool operationsRemainedQuantityGreater(const Operation& l, const Operation& r)
{
    return l.remainedQuantity > r.remainedQuantity;
}

inline bool operationsPaymentGreater(const Operation& l, const Operation& r)
{
    return l.payment > r.payment;
}

inline bool operationsCommissionGreater(const Operation& l, const Operation& r)
{
    return l.commission > r.commission;
}

inline bool operationsYieldGreater(const Operation& l, const Operation& r)
{
    return l.yield > r.yield;
}

inline bool operationsYieldWithCommissionGreater(const Operation& l, const Operation& r)
{
    return l.yieldWithCommission > r.yieldWithCommission;
}

inline bool operationsYieldWithCommissionPercentGreater(const Operation& l, const Operation& r)
{
    return l.yieldWithCommissionPercent > r.yieldWithCommissionPercent;
}

inline bool operationsTotalYieldWithCommissionGreater(const Operation& l, const Operation& r)
{
    return l.totalYieldWithCommission > r.totalYieldWithCommission;
}

inline bool operationsTotalYieldWithCommissionPercentGreater(const Operation& l, const Operation& r)
{
    return l.totalYieldWithCommissionPercent > r.totalYieldWithCommissionPercent;
}

inline bool operationsRemainedMoneyGreater(const Operation& l, const Operation& r)
{
    return l.remainedMoney > r.remainedMoney;
}

inline bool operationsTotalMoneyGreater(const Operation& l, const Operation& r)
{
    return l.totalMoney > r.totalMoney;
}
