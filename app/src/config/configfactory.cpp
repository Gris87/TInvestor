#include "configfactory.h"

#include <QDebug>

#include "src/config/config.h"



ConfigFactory::ConfigFactory()
{
    qDebug() << "Create ConfigFactory";
}

ConfigFactory::~ConfigFactory()
{
    qDebug() << "Destroy ConfigFactory";
}

IConfig* ConfigFactory::newInstance(IDecisionMakerConfigFactory *decisionMakerConfigFactory, QObject *parent)
{
    return new Config(decisionMakerConfigFactory, parent);
}
