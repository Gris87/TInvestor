#pragma once



#include "src/decisions/idecisionmaker.h"

#include "src/config/iconfig.h"
#include "src/decisions/iactiondecision.h"
#include "src/storage/user/iuserstorage.h"



class DecisionMaker : public IDecisionMaker
{
public:
    DecisionMaker(
        IConfig*                       config,
        IUserStorage*                  userStorage,
        const QList<IActionDecision*>& buyDecisions,
        const QList<IActionDecision*>& sellDecisions
    );
    ~DecisionMaker() override;

    DecisionMaker(const DecisionMaker& another)            = delete;
    DecisionMaker& operator=(const DecisionMaker& another) = delete;

    InstrumentsForTrading makeDecision(
        qint64 timestamp, const Portfolio& portfolio, const QList<Stock*>& stocks, int keepMoney, bool dateRange
    ) override;

private:
    void updateStocksMap(const QList<Stock*>& stocks);
    void splitStocks(
        const Portfolio& portfolio, const QList<Stock*>& stocks, QList<Stock*>& stocksForBuy, QList<Stock*>& stocksForSell
    );
    void
    makeBuyDecisions(qint64 timestamp, QList<Stock*>& stocksForBuy, int keepMoney, bool dateRange, InstrumentsForTrading& res);
    void makeSellDecisions(qint64 timestamp, QList<Stock*>& stocksForSell, bool dateRange, InstrumentsForTrading& res);

    IConfig*                mConfig;
    IUserStorage*           mUserStorage;
    QList<IActionDecision*> mBuyDecisions;
    QList<IActionDecision*> mSellDecisions;
    QMap<QString, Stock*>   mStocksMap;
};
