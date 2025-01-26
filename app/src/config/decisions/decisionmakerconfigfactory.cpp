#include "decisionmakerconfigfactory.h"

#include <QDebug>

#include "src/config/decisions/decisionmakerconfig.h"



DecisionMakerConfigFactory::DecisionMakerConfigFactory()
{
    qDebug() << "Create DecisionMakerConfigFactory";
}

DecisionMakerConfigFactory::~DecisionMakerConfigFactory()
{
    qDebug() << "Destroy DecisionMakerConfigFactory";
}

IDecisionMakerConfig* DecisionMakerConfigFactory::newInstance()
{
    return new DecisionMakerConfig();
}
