#pragma once



#include "src/domain/log/logentry.h"
#include "src/domain/operation/operation.h"
#include "src/domain/portfolio/portfolio.h"
#include "src/domain/quantityandcost/quantityandcost.h"
#include "src/domain/trading/tradinginfo.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/logos/ilogosstorage.h"
#include "src/storage/user/iuserstorage.h"



void simulateTrading(
    qint64                            timestamp,
    IInstrumentsStorage*              instrumentsStorage,
    ILogosStorage*                    logosStorage,
    IUserStorage*                     userStorage,
    const InstrumentsForTrading&      instrumentsForTrading,
    double                            startMoney,
    double&                           totalMoney,
    QList<Operation>&                 operations,
    QList<LogEntry>&                  entries,
    Portfolio&                        portfolio,
    QuantityAndCostDoubleInstruments& instruments
);
