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

InstrumentsForTrading DecisionMaker::makeDecision(const Portfolio& /*portfolio*/, const QList<Stock*>& stocks, int /*keepMoney*/)
{
    InstrumentsForTrading res;

    // TODO: Implement

    // TODO: Remove it [BEGIN]
    for (Stock* stock : stocks)
    {
        if (stock->meta.instrumentId == "48bd9002-43be-4528-abf4-dc8135ad4550")
        {
            const bool  needToBuy = QTime::currentTime().minute() % 2 == 0;
            TradingInfo tradingInfo;

            if (needToBuy)
            {
                tradingInfo.price        = stock->lastPrice();
                tradingInfo.expectedCost = 1000.0; // NOLINT(readability-magic-numbers)
                tradingInfo.cause        = "I want to buy";
            }
            else
            {
                tradingInfo.price        = stock->lastPrice();
                tradingInfo.expectedCost = 0.0;
                tradingInfo.cause        = "I want to sell";
            }

            res[stock->meta.instrumentId] = tradingInfo;

            break;
        }
    }
    // TODO: Remove it [END]

    return res;
}
