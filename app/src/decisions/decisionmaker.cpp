#include "src/decisions/decisionmaker.h"

#include <QDebug>



DecisionMaker::DecisionMaker() :
    IDecisionMaker()
{
    qDebug() << "Create DecisionMaker";
}

DecisionMaker::~DecisionMaker()
{
    qDebug() << "Destroy DecisionMaker";
}
