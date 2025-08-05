#pragma once



#include "src/decisions/idecisionmaker.h"

#include "src/config/iconfig.h"
#include "src/decisions/iactiondecision.h"
#include "src/storage/instruments/iinstrumentsstorage.h"
#include "src/storage/user/iuserstorage.h"



struct StockWithAvgPrice
{
    explicit StockWithAvgPrice(Stock* _stock, float _avgPrice) :
        stock(_stock),
        avgPrice(_avgPrice)
    {
    }

    Stock* stock;
    float  avgPrice;
};



class DecisionMaker : public IDecisionMaker
{
public:
    DecisionMaker(
        IInstrumentsStorage*           instrumentsStorage,
        IUserStorage*                  userStorage,
        const QList<IActionDecision*>& buyDecisions,
        const QList<IActionDecision*>& sellDecisions
    );
    ~DecisionMaker() override;

    DecisionMaker(const DecisionMaker& another)            = delete;
    DecisionMaker& operator=(const DecisionMaker& another) = delete;

    InstrumentsForTrading makeDecision(
        QThread*             parentThread,
        qint64               timestamp,
        IConfig*             config,
        const Portfolio&     portfolio,
        const QList<Stock*>& stocks,
        bool                 autoPilot,
        int                  keepMoney,
        bool                 dateRange,
        bool                 useParallel
    ) override;

private:
    IDecisionMakerConfig* chooseDecisionConfig(IConfig* config, bool autoPilot);
    void                  updateStocksMap(const QList<Stock*>& stocks);
    void
    getStocksWithAvgPrice(const Portfolio& portfolio, const QList<Stock*>& stocks, QList<StockWithAvgPrice>& stocksWithAvgPrice);
    void makeDecisions(
        QThread*                  parentThread,
        IConfig*                  config,
        IDecisionMakerConfig*     decisionConfig,
        qint64                    timestamp,
        const Portfolio&          portfolio,
        QList<StockWithAvgPrice>& stocksWithAvgPrice,
        int                       keepMoney,
        bool                      dateRange,
        bool                      useParallel,
        InstrumentsForTrading&    res
    );
    void calculateTotalCostAndMoney(const Portfolio& portfolio, double& totalCost, double& money);

    IInstrumentsStorage*    mInstrumentsStorage;
    IUserStorage*           mUserStorage;
    QList<IActionDecision*> mBuyDecisions;
    QList<IActionDecision*> mSellDecisions;
    QMap<QString, Stock*>   mStocksMap;
};
