#include "src/decisions/decisionmaker.h"

#include <QDebug>



DecisionMaker::DecisionMaker(IConfig* config, IUserStorage* userStorage) :
    IDecisionMaker(),
    mConfig(config),
    mUserStorage(userStorage)
{
    qDebug() << "Create DecisionMaker";
}

DecisionMaker::~DecisionMaker()
{
    qDebug() << "Destroy DecisionMaker";
}

InstrumentsForTrading
DecisionMaker::makeDecision(const Portfolio& /*portfolio*/, const QList<Stock*>& /*stocks*/, int /*keepMoney*/)
{
    InstrumentsForTrading res;

    // TODO: Implement

    // TODO: Remove it [BEGIN]
    const bool  needToBuy = QTime::currentTime().minute() % 2 == 0;
    TradingInfo tradingInfo;

    if (needToBuy)
    {
        tradingInfo.expectedCost = 1000.0;
        tradingInfo.cause        = "I want to buy";
    }
    else
    {
        tradingInfo.expectedCost = 0.0;
        tradingInfo.cause        = "I want to sell";
    }

    res["48bd9002-43be-4528-abf4-dc8135ad4550"] = tradingInfo;
    // TODO: Remove it [END]

    return res;
}
