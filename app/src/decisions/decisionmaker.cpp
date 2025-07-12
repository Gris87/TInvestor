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

void DecisionMaker::makeDecision(const Portfolio& /*portfolio*/, const QList<Stock*>& /*stocks*/)
{
    // TODO: Implement
}
