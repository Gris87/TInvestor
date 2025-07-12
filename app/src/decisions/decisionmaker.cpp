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

InstrumentsForTrading DecisionMaker::makeDecision(const Portfolio& /*portfolio*/, const QList<Stock*>& /*stocks*/)
{
    InstrumentsForTrading res;

    // TODO: Implement

    return res;
}
